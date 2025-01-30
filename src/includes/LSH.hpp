#ifndef LSH_HPP
#define LSH_HPP

#include "PCB.hpp"
#include "RAM.hpp"
#include <unordered_map>
#include <vector>
#include <queue>

using namespace std;

class LSH
{
public:
    static void organizarPorSimilaridade(queue<PCB *> &filaProntos, RAM &ram);

private:
    static size_t calcularHashLSH(PCB *processo, RAM &ram);
};

#endif
