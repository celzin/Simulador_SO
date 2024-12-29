#ifndef CORE_HPP
#define CORE_HPP

#include "Registers.hpp"
#include "UnidadeControle.hpp"
#include "RAM.hpp"
#include "Disco.hpp"
#include "PCB.hpp"

#include <thread>

class Core {
public:
    int id; // Identificador único do núcleo
    Registers regs;
    UnidadeControle uc;
    int PC;
    RAM& ram;
    Disco& disco;
    int Clock;
    vector<pair<string,int>>& processosCarregados; 
    PCB* processoAtual; // PCB do processo em execução

    Core(int id, Registers& regs, RAM& ram, Disco& disco, vector<pair<string,int>>& processosCarregados);
    void executarProcesso(); // Executa o processo atual
    void setProcesso(PCB* processo); // Configura o processo a ser executado
};

#endif