#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "Disco.hpp"
#include "Instruction.hpp"
#include "Registers.hpp"
#include "ULA.hpp"
#include "RAM.hpp"
#include "InstructionDecode.hpp"

class Pipeline
{
private:
    ULA ula;

public:
    Pipeline();
    void PipelineProcess(Registers& regs, RAM& ram, int& PC, Disco& Disco, int& Clock, int& instructionAddress);
    Instruction InstructionFetch(RAM& ram, int endereco);
    void Wb(const DecodedInstruction& decoded, int& resultado, RAM& ram, Disco& disco, int& Clock);
    void MemoryAccess(const DecodedInstruction& decoded, int resultado, Registers& regs, int& Clock);
    void Execute(const DecodedInstruction& decoded, Registers& regs, RAM& ram, int& PC, Disco& Disco, int& Clock);
};

#endif