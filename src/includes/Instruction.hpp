#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include "Opcode.hpp"
#include <ostream>
#include <cstdint>

struct Instruction {
    Opcode op;
    int Destiny_Register;
    int Register_1;
    int Register_2;

    Instruction(Opcode opc, int rd, int r1, int r2);
    Instruction();

    int toInt() const;
    static Instruction fromInt(int value);
    friend std::ostream& operator<<(std::ostream& os, const Instruction& instr);
};

#endif
