#ifndef UNIDADECONTROLE_HPP
#define UNIDADECONTROLE_HPP

#include "Registers.hpp"
#include "RAM.hpp"
#include "Disco.hpp"
#include "Pipeline.hpp"

using namespace std;

class UnidadeControle {
private:
    Pipeline pipeline;

public:
    void executarInstrucao(Registers& regs, RAM& ram, int& PC, Disco& disco, int& Clock, int instructionAddress);
};

#endif
