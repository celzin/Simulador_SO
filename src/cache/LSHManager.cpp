#include "../includes/LSHManager.hpp"

int LSHManager::calcularHash(PCB *pcb)
{
    int hash = 0;
    for (const auto &[op, freq] : pcb->getPerfilInstrucoes())
    {
        hash ^= (static_cast<int>(op) * freq);
    }
    return hash % NUM_BUCKETS;
}

void LSHManager::inserirNoBucket(PCB *pcb)
{
    int hash = calcularHash(pcb);
    buckets[hash].push(pcb);
    pcb->setHash(hash);
}

PCB *LSHManager::buscarProcessosSimilares()
{
    for (auto &[hash, fila] : buckets)
    {
        if (!fila.empty())
        {
            PCB *processo = fila.front();
            fila.pop();
            return processo;
        }
    }
    return nullptr;
}
