#ifndef CORE_HPP
#define CORE_HPP

#include "Opcode.hpp"
#include "Instruction.hpp"
#include "Registers.hpp"
#include "ULA.hpp"
#include "UnidadeControle.hpp"
#include "RAM.hpp"
#include "InstructionDecode.hpp"
#include "Pipeline.hpp"
#include "Disco.hpp"
#include "Pipeline.hpp"

class Core {
public:
    Registers regs;
    UnidadeControle uc;
    int PC;
    RAM& ram;
    Disco& disco;
    int Clock;

    Core(RAM& ram, Disco& disco);
    void activate();
};

#endif