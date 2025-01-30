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

void LSH::organizarPorSimilaridade(queue<PCB *> &filaProntos, RAM &ram)
{
    unordered_map<size_t, vector<PCB *>> bucketsLSH;
    vector<PCB *> filaTemporaria;

    while (!filaProntos.empty())
    {
        PCB *processo = filaProntos.front();
        filaProntos.pop();

        size_t hashProcesso = calcularHashLSH(processo, ram);
        bucketsLSH[hashProcesso].push_back(processo);
    }

    for (auto &[_, bucket] : bucketsLSH)
    {
        for (PCB *processo : bucket)
        {
            filaTemporaria.push_back(processo);
        }
    }

    for (PCB *processo : filaTemporaria)
    {
        filaProntos.push(processo);
    }
}
