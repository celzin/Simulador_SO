#ifndef UNIDADECONTROLE_HPP
#define UNIDADECONTROLE_HPP

#include <iostream>
#include "Instruction.hpp"
#include "Registers.hpp"
#include "ULA.hpp"
#include "RAM.hpp"
#include "InstructionDecode.hpp"
#include "Disco.hpp"
#include "Pipeline.hpp"

using namespace std;

class UnidadeControle {
private:
    Pipeline pipeline;

public:
    void executarInstrucao(Registers& regs, RAM& ram, int& PC, Disco& disco, int& Clock);
};

#endif
