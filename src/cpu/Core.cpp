#include "../includes/Core.hpp"

Core::Core(RAM& ram, Disco& disco, Escalonador& escalonador) 
    : ram(ram), disco(disco), PC(0), Clock(0), escalonador(escalonador) {}

void Core::activate() {
    while (!escalonador.filaVazia()) {
        PCB* pcb = escalonador.obterProximoProcesso();

        if (pcb == nullptr) {
            std::cerr << "[Núcleo " << std::this_thread::get_id() << "] Erro: Nenhum processo disponível para execução.\n";
            return;
        }

        // Restaurar o estado do processo
        std::vector<int> pipelineState;
        pcb->restaurarEstado(pipelineState);

        pcb->atualizarEstado(EXECUCAO);
        std::cout << "[Núcleo " << std::this_thread::get_id() << "] Iniciando execução do processo [PID: " << pcb->pid << "]\n";
        pcb->exibirPCB(); // Imprime o estado inicial do PCB

        while (!pcb->quantumExpirado()) {
            // Valida se o PC está dentro do limite de instruções antes do fetch
            if (pcb->PC < pcb->getEnderecoBaseInstrucoes() || pcb->PC > pcb->getLimiteInstrucoes()) {
                std::cout << "[Núcleo " << std::this_thread::get_id() << "] Processo " << pcb->pid 
                          << " atingiu o limite de instruções (PC: " << pcb->PC 
                          << ", Base: " << pcb->getEnderecoBaseInstrucoes() 
                          << ", Limite: " << pcb->getLimiteInstrucoes() << ").\n";
                pcb->atualizarEstado(FINALIZADO);
                break;
            }

            // Busca a próxima instrução
            Instruction instr = pipeline.InstructionFetch(ram, pcb->PC, pcb);
            if (instr.op == NOP) {
                std::cerr << "[Núcleo " << std::this_thread::get_id() << "] Instrução inválida no PC: " << pcb->PC << ". Encerrando execução.\n";
                pcb->atualizarEstado(FINALIZADO);
                break;
            }

            // Executa a instrução
            uc.executarInstrucao(instr, pcb->registradores, ram, pcb->PC, disco, Clock, *pcb);

            // Incrementa o PC
            pcb->PC += 1; // Incremento em unidades para acompanhar a RAM

            // Decrementa o quantum
            pcb->decrementarQuantum();
        }

        // Salvar o estado do processo
        pcb->salvarEstado(pipeline.getPipelineState());
        std::cout << "[Núcleo " << std::this_thread::get_id() << "] Finalizando execução do processo [PID: " << pcb->pid << "]:\n";
        pcb->exibirPCB(); // Exibe o estado final do PCB

        // Gerenciamento de estados
        if (pcb->verificarEstado(FINALIZADO)) {
            std::cout << "[Núcleo " << std::this_thread::get_id() << "] Processo [PID: " << pcb->pid << "] finalizado.\n";
        } else if (pcb->quantumExpirado()) {
            pcb->atualizarEstado(PRONTO);
            escalonador.adicionarProcesso(pcb);
            std::cout << "[Núcleo " << std::this_thread::get_id() << "] Quantum expirado para o processo [PID: " << pcb->pid << "]. Retornando à fila de prontos.\n";
        } else if (pcb->verificarEstado(BLOQUEADO)) {
            std::cout << "[Núcleo " << std::this_thread::get_id() << "] Processo [PID: " << pcb->pid << "] bloqueado.\n";
        }
    }
}


void Core::run() {
    std::cout << "\n" << "Executando o núcleo (Thread) com PC: " << PC << "\n\n";
    activate();
}

void Core::validateMemoryAccess(PCB* processo, int endereco) {
    if (!processo->verificarAcessoMemoria(endereco) || ram.isReserved(endereco)) {
        std::cerr << "[Erro] Acesso inválido à memória no endereço " << endereco << " pelo processo " << processo->pid << "\n";
        processo->atualizarEstado(BLOQUEADO); // Bloqueia o processo caso o acesso seja inválido
    }
}

void Core::gerenciarRecursos(PCB* processo) {
    // Simula uma verificação de recursos
    std::string recursoNecessario = "teclado"; // Exemplo de recurso
    if (!processo->verificarRecurso(recursoNecessario)) {
        processo->associarRecurso(recursoNecessario, true); // Aloca o recurso
    } else {
        processo->atualizarEstado(BLOQUEADO); // Bloqueia o processo caso o recurso esteja indisponível
    }
}
