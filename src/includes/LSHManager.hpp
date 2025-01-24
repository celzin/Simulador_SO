#ifndef LSH_MANAGER_HPP
#define LSH_MANAGER_HPP

#include "PCB.hpp"
#include <unordered_map>
#include <queue>

class LSHManager
{
private:
    static const int NUM_BUCKETS = 16;
    std::unordered_map<int, std::queue<PCB *>> buckets; // Hash -> Fila de processos similares

public:
    int calcularHash(PCB *pcb);
    void inserirNoBucket(PCB *pcb);
    PCB *buscarProcessosSimilares();
};

#endif
