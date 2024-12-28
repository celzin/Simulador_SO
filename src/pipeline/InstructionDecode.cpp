#include "../includes/InstructionDecode.hpp"

DecodedInstruction InstructionDecode(const Instruction& instr, const Registers& regs) {
    DecodedInstruction decoded;
    decoded.opcode = instr.op;
    decoded.destiny = instr.Destiny_Register;
    decoded.value1 = regs.get(instr.Register_1);
    decoded.value2 = regs.get(instr.Register_2);

    // cout << "Decodificando Instrução: Opcode=" << decoded.opcode
    //     << ", Destino=R" << decoded.destiny
    //     << ", Operando 1 (R" << instr.Register_1 << ")=" << decoded.value1
    //     << ", Operando 2 (R" << instr.Register_2 << ")=" << decoded.value2 << endl;

    return decoded;
}
