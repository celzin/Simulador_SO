#ifndef CACHE_HPP
#define CACHE_HPP

#include "PCB.hpp"
#include <unordered_map>
#include <queue>

class Cache
{
private:
    std::unordered_map<int, int> cache; // Hash do processo -> Resultado
    std::queue<int> ordemFIFO;          // FIFO para remoção
    static const int TAMANHO_CACHE = 8; // Tamanho fixo

public:
    void armazenarResultado(PCB *pcb, int resultado);
    int buscarResultado(PCB *pcb);
};

#endif
