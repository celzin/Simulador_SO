#include "../includes/enums/PoliticasEscalonamento.hpp"

PCB *PoliticasEscalonamentoHandler::selecionarProcessoFCFS(queue<PCB *> &filaProntos, ofstream &outfile)
{
    PCB *processo = filaProntos.front();
    filaProntos.pop();

    outfile
        << "\n************************************************************************************************************************\n";
    outfile << "[Escalonador][FCFS] Retirando o processo " << processo->pid << " da fila de PRONTOS. Tamanho: " << processo->tempoEstimado << ".\n";

    return processo;
}

PCB *PoliticasEscalonamentoHandler::selecionarProcessoSJF(queue<PCB *> &filaProntos, ofstream &outfile)
{
    vector<PCB *> tempoQueue;
    PCB *processoMenorTempo = nullptr;

    while (!filaProntos.empty())
    {
        PCB *atual = filaProntos.front();
        filaProntos.pop();
        tempoQueue.push_back(atual);

        if (!processoMenorTempo || atual->tempoEstimado < processoMenorTempo->tempoEstimado)
        {
            processoMenorTempo = atual;
        }
    }

    for (PCB *pcb : tempoQueue)
    {
        if (pcb != processoMenorTempo)
        {
            filaProntos.push(pcb);
        }
    }

    outfile << "\n************************************************************************************************************************\n";
    outfile << "[Escalonador][SJF] Retirando o processo " << processoMenorTempo->pid
            << " da fila de PRONTOS. Tamanho: " << processoMenorTempo->tempoEstimado << ".\n";

    return processoMenorTempo;
}

PCB *PoliticasEscalonamentoHandler::selecionarProcessoPrioridade(queue<PCB *> &filaProntos, ofstream &outfile)
{
    vector<PCB *> prioridadeQueue;
    PCB *processoMaiorPrioridade = nullptr;

    while (!filaProntos.empty())
    {
        PCB *atual = filaProntos.front();
        filaProntos.pop();
        prioridadeQueue.push_back(atual);

        if (!processoMaiorPrioridade || atual->prioridade > processoMaiorPrioridade->prioridade)
        {
            processoMaiorPrioridade = atual;
        }
    }

    for (PCB *pcb : prioridadeQueue)
    {
        if (pcb != processoMaiorPrioridade)
        {
            filaProntos.push(pcb);
        }
    }

    outfile << "\n************************************************************************************************************************\n";
    outfile << "[Escalonador][Prioridade] Retirando o processo " << processoMaiorPrioridade->pid
            << " da fila de PRONTOS. Prioridade [Nível " << processoMaiorPrioridade->prioridade << "][";
    for (int j = 0; j < processoMaiorPrioridade->prioridade; ++j)
    {
        outfile << "★";
    }

    return processoMaiorPrioridade;
}

PCB *PoliticasEscalonamentoHandler::selecionarProcessoSimilaridade(queue<PCB *> &filaProntos, Cache &cache, RAM &ram, ofstream &outfile)
{
    vector<PCB *> candidatos;
    PCB *melhorProcesso = nullptr;
    int maxReuso = 0;

    // Avalia cada processo e verifica quantas instruções já estão na Cache
    while (!filaProntos.empty())
    {
        PCB *atual = filaProntos.front();
        filaProntos.pop();
        candidatos.push_back(atual);

        int reusoInstrucao = 0;
        for (int i = atual->getEnderecoBaseInstrucoes(); i < atual->getLimiteInstrucoes(); ++i)
        {
            Instruction instr = ram.fetchInstruction(i); // Agora usamos a RAM corretamente
            if (cache.contains(instr))
            {
                reusoInstrucao++;
            }
        }

        // Atualiza o processo que pode reutilizar mais instruções da Cache
        if (reusoInstrucao > maxReuso)
        {
            maxReuso = reusoInstrucao;
            melhorProcesso = atual;
        }
    }

    // Se nenhum processo tiver similaridade, usa FCFS como fallback
    if (!melhorProcesso)
    {
        melhorProcesso = candidatos.front();
    }

    // Reinsere os processos restantes na fila
    for (PCB *pcb : candidatos)
    {
        if (pcb != melhorProcesso)
        {
            filaProntos.push(pcb);
        }
    }

    outfile << "[Escalonador][SIMILARIDADE] Processo " << melhorProcesso->pid
            << " selecionado com " << maxReuso << " instruções já armazenadas na Cache.\n";

    return melhorProcesso;
}