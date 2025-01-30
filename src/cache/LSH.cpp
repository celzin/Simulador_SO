#include "../includes/LSH.hpp"
#include <string>
#include <functional>

size_t LSH::calcularHashLSH(PCB *processo, RAM &ram)
{
    size_t hashFinal = 0;
    std::hash<string> hashFn;

    for (int i = processo->getEnderecoBaseInstrucoes(); i < processo->getLimiteInstrucoes(); ++i)
    {
        Instruction instrucao = ram.fetchInstruction(i);
        string instrucaoStr = to_string(instrucao.op) + "_" +
                              to_string(instrucao.Destiny_Register) + "_" +
                              to_string(instrucao.Register_1) + "_" +
                              to_string(instrucao.Register_2);
        hashFinal ^= hashFn(instrucaoStr);
    }

    return hashFinal;
}

int LSH::calcularSimilaridade(PCB *p1, PCB *p2, RAM &ram)
{
    unordered_map<string, int> instrucoesP1;
    int iguais = 0;

    // Armazena as instruções de P1 em um mapa
    for (int i = p1->getEnderecoBaseInstrucoes(); i < p1->getLimiteInstrucoes(); i++)
    {
        Instruction instrucao = ram.fetchInstruction(i);
        string instrucaoStr = to_string(instrucao.op) + "_" +
                              to_string(instrucao.Destiny_Register) + "_" +
                              to_string(instrucao.Register_1) + "_" +
                              to_string(instrucao.Register_2);
        instrucoesP1[instrucaoStr]++;
    }

    // Conta quantas instruções de P2 também existem em P1
    for (int i = p2->getEnderecoBaseInstrucoes(); i < p2->getLimiteInstrucoes(); i++)
    {
        Instruction instrucao = ram.fetchInstruction(i);
        string instrucaoStr = to_string(instrucao.op) + "_" +
                              to_string(instrucao.Destiny_Register) + "_" +
                              to_string(instrucao.Register_1) + "_" +
                              to_string(instrucao.Register_2);

        if (instrucoesP1[instrucaoStr] > 0)
        {
            iguais++;
            instrucoesP1[instrucaoStr]--; // Evita contar a mesma instrução várias vezes
        }
    }

    return iguais;
}

void LSH::organizarPorSimilaridade(queue<PCB *> &filaProntos, RAM &ram)
{
    vector<PCB *> processos;
    while (!filaProntos.empty())
    {
        processos.push_back(filaProntos.front());
        filaProntos.pop();
    }

    if (processos.empty())
        return;

    cout << "\n[LSH] Fila original dos processos:\n";
    for (PCB *p : processos)
        cout << "Processo " << p->pid << "\n";

    // Construção da matriz de similaridade
    unordered_map<int, unordered_map<int, int>> matrizSimilaridade;
    for (size_t i = 0; i < processos.size(); i++)
    {
        for (size_t j = i + 1; j < processos.size(); j++)
        {
            int similaridade = calcularSimilaridade(processos[i], processos[j], ram);
            matrizSimilaridade[processos[i]->pid][processos[j]->pid] = similaridade;
            matrizSimilaridade[processos[j]->pid][processos[i]->pid] = similaridade;
        }
    }

    cout << "\n[LSH] Matriz de Similaridade entre Processos:\n";
    for (size_t i = 0; i < processos.size(); i++)
    {
        for (size_t j = i + 1; j < processos.size(); j++)
        {
            cout << "P" << processos[i]->pid << " <-> P" << processos[j]->pid
                 << " : " << matrizSimilaridade[processos[i]->pid][processos[j]->pid] << " instruções iguais.\n";
        }
    }

    vector<PCB *> filaOrdenada;
    vector<bool> usados(processos.size(), false);

    // Escolhe o primeiro processo arbitrariamente
    filaOrdenada.push_back(processos[0]);
    usados[0] = true;

    // Ordena escolhendo sempre o mais similar ao último adicionado
    while (filaOrdenada.size() < processos.size())
    {
        PCB *ultimo = filaOrdenada.back();
        int indiceMaisSimilar = -1;
        int maiorSimilaridade = -1;

        for (size_t i = 0; i < processos.size(); i++)
        {
            if (!usados[i])
            {
                int similaridade = matrizSimilaridade[ultimo->pid][processos[i]->pid];
                if (similaridade > maiorSimilaridade)
                {
                    maiorSimilaridade = similaridade;
                    indiceMaisSimilar = i;
                }
            }
        }

        if (indiceMaisSimilar != -1)
        {
            filaOrdenada.push_back(processos[indiceMaisSimilar]);
            usados[indiceMaisSimilar] = true;
        }
        else
        {
            // Se não houver similaridade, escolhe qualquer processo não usado
            for (size_t i = 0; i < processos.size(); i++)
            {
                if (!usados[i])
                {
                    filaOrdenada.push_back(processos[i]);
                    usados[i] = true;
                    break;
                }
            }
        }
    }

    cout << "\n[LSH] Ordem final dos processos após organização:\n";
    for (PCB *p : filaOrdenada)
        cout << "Processo " << p->pid << "\n";

    // Reconstituir a fila original na nova ordem
    for (PCB *processo : filaOrdenada)
    {
        filaProntos.push(processo);
    }
}