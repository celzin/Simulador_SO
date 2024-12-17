#include "../includes/ProcessControlBlock.hpp"

void ProcessControlBlock::saveState(int pc, const Registers& registers) {
    PC = pc;
    regs = registers;
}

void ProcessControlBlock::restoreState(int& pc, Registers& registers) {
    pc = PC;
    registers = regs;
}
