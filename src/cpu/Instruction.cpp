#include "../includes/Instruction.hpp"

Instruction::Instruction(Opcode opc, int rd, int r1, int r2)
    : op(opc), Destiny_Register(rd), Register_1(r1), Register_2(r2) {}

Instruction::Instruction() 
    : op(ADD), Destiny_Register(0), Register_1(0), Register_2(0) {}

std::ostream& operator<<(std::ostream& os, const Instruction& instr) {
    os << "Opcode: " << instr.op << ", Destino: R" << instr.Destiny_Register
       << ", R1: " << instr.Register_1 << ", R2: " << instr.Register_2;
    return os;
}

// Converte a instrução para um número inteiro
int Instruction::toInt() const {
    return (static_cast<int>(op) << 24) | (Destiny_Register << 16) | 
           (Register_1 << 8) | Register_2;
}

// Reconstrói uma instrução a partir de um número inteiro
Instruction Instruction::fromInt(int value) {
    Opcode opc = static_cast<Opcode>((value >> 24) & 0xFF);
    int rd = (value >> 16) & 0xFF;
    int r1 = (value >> 8) & 0xFF;
    int r2 = value & 0xFF;
    return Instruction(opc, rd, r1, r2);
}