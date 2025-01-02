#ifndef PROCESS_MANAGER_HPP
#define PROCESS_MANAGER_HPP

#include "RAM.hpp"
#include "Disco.hpp"
#include "PCB.hpp"

#include <vector>
#include <string>
#include <iostream>

using namespace std;

class ProcessManager {
public:
    // Criar pcbs a partir de arquivos de instruções
    static vector<PCB*> createPCBs(Disco& disco, RAM& ram, Registers& regs, const vector<string>& arquivosInstrucoes);
};

#endif
