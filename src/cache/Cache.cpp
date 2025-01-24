#include "../includes/Cache.hpp"

void Cache::armazenarResultado(PCB *pcb, int resultado)
{
    int hash = pcb->getHash();
    if (cache.size() >= TAMANHO_CACHE)
    {
        int chaveMaisAntiga = ordemFIFO.front();
        ordemFIFO.pop();
        cache.erase(chaveMaisAntiga);
    }
    cache[hash] = resultado;
    ordemFIFO.push(hash);
}

int Cache::buscarResultado(PCB *pcb)
{
    int hash = pcb->getHash();
    return cache.count(hash) ? cache[hash] : -1; // Retorna -1 se não encontrado
}
