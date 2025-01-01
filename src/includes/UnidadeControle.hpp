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
#include "PCB.hpp"

using namespace std;

class UnidadeControle {
private:
    Pipeline pipeline;

public:
    void executarInstrucao(const Instruction& instr, Registers& regs, RAM& ram, int& PC, Disco& disco, int& Clock, PCB& pcb);
};

#endif
