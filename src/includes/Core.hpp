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
#include "PCB.hpp"

class Core {
public:
    Registers regs;
    UnidadeControle uc;
    int PC;
    RAM& ram;
    Disco& disco;
    int Clock;

    ProcessControlBlock* currentProcess; // PCB associado ao Core
    Core(RAM& ram, Disco& disco);
    void activate();
    void loadProcess(ProcessControlBlock* pcb); // Carrega um PCB no Core
};