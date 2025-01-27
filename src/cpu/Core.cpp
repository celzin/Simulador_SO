#include "../includes/Core.hpp"

#define OUTPUT_DIR "data/logs"

Core::Core(RAM &ram, Disco &disco, Escalonador &escalonador, Cache *cache)
    : ram(ram), disco(disco), escalonador(escalonador), cache(cache), PC(0), Clock(0) {}

void Core::activate(ofstream &outfile)
{
    while (escalonador.temProcessosProntos())
    {
        auto inicioOcupado = chrono::high_resolution_clock::now();
        PCB *pcb = escalonador.obterProximoProcesso(outfile);

        outfile << "Processo " << pcb->pid
                << ": Base (Instruções) = " << pcb->enderecoBaseInstrucoes
                << ", Limite (Instruções) = " << pcb->enderecoLimiteInstrucoes
                << ", PC (Inicial) = " << pcb->PC
                << ", Quantum = " << pcb->quantumProcesso
                << endl;

        if (pcb == nullptr)
        {
            outfile << "[Núcleo " << this_thread::get_id() << "] Erro: Nenhum Processo disponível para execução.\n";
            return;
        }

        // **Cálculo do tempo de espera e tempo de retorno
        int tempoExecutado = 0;
        int tempoEspera = tempoAtual;

        // Restaurar o estado do processo
        auto pipelineState = pipeline.getPipelineState();
        pcb->restaurarEstado(pipelineState, outfile);

        pcb->atualizarEstado(EXECUCAO, outfile);
        outfile << "[Núcleo " << this_thread::get_id() << "] Iniciando execução do Processo [PID: " << pcb->pid << "]\n";
        outfile << "\n=============== [PCB PRÉ-EXECUÇÃO]:";
        pcb->exibirPCB(outfile); // Imprime o estado inicial do PCB

        // Loop principal de execução de processo
        while (true)
        {
            // Verifica se o quantum expirou
            if (pcb->quantumExpirado())
            {
                outfile << "[Quantum Expirado] Processo " << pcb->pid << " NÃO TERMINOU. Quantum Alocado Insuficitente! Vai para o Fim da Fila!\n\n";
                pcb->salvarEstado(pipeline.getPipelineState()); // Salva o estado completo
                break;
            }

            // Valida se o PC está dentro do limite de instruções antes do fetch
            if (pcb->PC >= pcb->getLimiteInstrucoes())
            {
                outfile << "\n[Núcleo " << this_thread::get_id() << "] Processo " << pcb->pid
                        << " executou todas as instruções (PC: " << pcb->PC
                        << ", Base: " << pcb->getEnderecoBaseInstrucoes()
                        << ", Limite: " << pcb->getLimiteInstrucoes() << ")\n";

                pcb->atualizarEstado(FINALIZADO, outfile);
                break;
            }

            // Busca a próxima instrução
            Instruction instr = pipeline.InstructionFetch(ram, pcb->PC, pcb);
            if (instr.op == NOP)
            {
                outfile << "[Núcleo " << this_thread::get_id() << "] Instrução inválida no PC: " << pcb->PC << ". Encerrando execução.\n";
                pcb->atualizarEstado(FINALIZADO, outfile);
                break;
            }

            // **Integração com a Cache**
            if (cache && cache->contains(instr))
            { // 🔹 Verifica se a instrução já está na Cache
                outfile << "[Cache] Instrução reutilizada da Cache no PC " << pcb->PC << ". Pulando execução.\n";
            }
            else
            {
                // Executa a instrução normalmente
                uc.executarInstrucao(instr, pcb->registradores, ram, pcb->PC, disco, Clock, *pcb, outfile);

                if (cache)
                { // 🔹 Armazena o resultado na Cache para reutilização futura
                    cache->insert(instr, pcb->registradores.get(instr.Destiny_Register));
                }
            }

            // Incrementa o PC
            pcb->PC += 1; // Incremento em unidades para acompanhar a RAM

            // Decrementa o quantum
            pcb->decrementarQuantum(outfile);
            tempoExecutado++; // Atualiza tempo de execução real
        }

        // **Corrigir tempo de retorno para processos preemptados**
        int tempoRetorno = tempoEspera + tempoExecutado; // Agora, considera só o tempo real executado
        // Atualizar métricas do núcleo
        tempoTotalEspera += tempoEspera;
        tempoTotalRetorno += tempoRetorno;
        processosExecutados++;
        tempoAtual += tempoExecutado; // **Atualizar tempo total do núcleo apenas com tempo real de execução**

        outfile << "[Núcleo " << this_thread::get_id()
                << "] Processo: " << pcb->pid
                << " | Tempo de Espera: " << tempoEspera
                << " | Tempo Executado: " << tempoExecutado
                << " | Tempo de Retorno: " << tempoRetorno
                << " | Execuções de Processos: " << processosExecutados << endl;

        // Salvar o estado do processo
        pcb->salvarEstado(pipeline.getPipelineState());
        outfile << "[Núcleo " << this_thread::get_id() << "] Encerrando a execução do Processo [PID: " << pcb->pid << "]\n";

        // Gerenciamento de estados
        if (pcb->verificarEstado(FINALIZADO))
        {
            outfile << "[Núcleo " << this_thread::get_id() << "] Processo [PID: " << pcb->pid << "] FINALIZADO.\n";
            outfile << "************************************************************************************************************************\n";
        }
        else if (pcb->quantumExpirado())
        {
            pcb->resetarQuantum(outfile);
            outfile << "=============== [PCB PÓS-EXECUÇÃO]:";
            pcb->exibirPCB(outfile); // Exibe PCB novamente
            outfile << "[Núcleo " << this_thread::get_id() << "] Quantum expirado para o Processo [PID: " << pcb->pid << "]" << endl;
            outfile << "Após ter seu Quantum reiniciado para [ " << pcb->quantumProcesso << " ] retorna à fila de PRONTOS" << endl;
            pcb->atualizarEstado(PRONTO, outfile);
            escalonador.adicionarProcesso(pcb, outfile);
        }
        else if (pcb->verificarEstado(BLOQUEADO))
        {
            outfile << "[Núcleo " << this_thread::get_id() << "] Processo [PID: " << pcb->pid << "] bloqueado.\n";
        }
        auto fimOcupado = chrono::high_resolution_clock::now();
        tempoOcupado += chrono::duration_cast<chrono::duration<double, milli>>(fimOcupado - inicioOcupado).count();
    }
    auto inicioOcioso = chrono::high_resolution_clock::now();
    this_thread::sleep_for(chrono::milliseconds(1));
    auto fimOcioso = chrono::high_resolution_clock::now();
    tempoOcioso += chrono::duration_cast<chrono::duration<double, milli>>(fimOcioso - inicioOcioso).count();
}

void Core::run()
{
    if (!filesystem::exists(OUTPUT_DIR))
    {
        filesystem::create_directory(OUTPUT_DIR);
    }

    ostringstream oss;
    oss << this_thread::get_id();
    string this_thread_id_str = oss.str();

    // Nome do output para este núcleo
    string filename = string(OUTPUT_DIR) + "/output_nucleo_" + this_thread_id_str + ".txt";
    ofstream outfile(filename, ios::out);

    if (!outfile.is_open())
    {
        cerr << "[Núcleo " << this_thread::get_id() << "] Erro ao criar arquivo de saída.\n";
        return;
    }

    outfile << "[Núcleo] ID: " << this_thread::get_id() << "\n";

    // cout << "\n" << "Executando o núcleo (Thread) com PC: " << PC << "\n\n";
    activate(outfile);
    exibirTempoCore(outfile);
    outfile.close();
}

void Core::exibirTempoCore(ofstream &outfile)
{
    outfile << fixed << setprecision(2);
    outfile << "\n=============== Estatísticas do Núcleo ================" << endl;
    outfile << "Núcleo ID: " << this_thread::get_id() << endl;
    outfile << "Tempo ocupado: " << tempoOcupado << " ms\n";
    outfile << "Tempo ocioso: " << tempoOcioso << " ms\n";

    if (processosExecutados > 0)
    {
        outfile << "Tempo médio de espera: " << (tempoTotalEspera / processosExecutados) << " unidades de tempo\n";
        outfile << "Tempo médio de retorno: " << (tempoTotalRetorno / processosExecutados) << " unidades de tempo\n";
    }
    else
    {
        outfile << "Tempo médio de espera: N/A\n";
        outfile << "Tempo médio de retorno: N/A\n";
    }

    outfile << "Número total de execuções de processos: " << processosExecutados << endl;

    if (tempoOcupado + tempoOcioso > 0)
    {
        outfile << "Taxa de utilização: " << (tempoOcupado / (tempoOcupado + tempoOcioso)) * 100 << " %\n";
    }
    else
    {
        outfile << "Taxa de utilização: 0.000 %\n";
    }

    outfile << "========================================================\n";
}

void Core::validateMemoryAccess(PCB *processo, int endereco, ofstream &outfile)
{
    if (!processo->verificarAcessoMemoria(endereco) || ram.isReserved(endereco))
    {
        outfile << "[Erro] Acesso inválido à memória no endereço " << endereco << " pelo Processo " << processo->pid << "\n";
        processo->atualizarEstado(BLOQUEADO, outfile); // Bloqueia o processo caso o acesso seja inválido
    }
}

void Core::gerenciarRecursos(PCB *processo, ofstream &outfile)
{
    // Simula uma verificação de recursos
    string recursoNecessario = "teclado"; // Exemplo de recurso
    if (!processo->verificarRecurso(recursoNecessario))
    {
        processo->associarRecurso(recursoNecessario, true); // Aloca o recurso
    }
    else
    {
        processo->atualizarEstado(BLOQUEADO, outfile); // Bloqueia o processo caso o recurso esteja indisponível
    }
}
