#include "../includes/UnidadeControle.hpp"

void UnidadeControle::executarInstrucao(Registers& regs, RAM& ram, int& PC, Disco& disco, int& Clock) {
    pipeline.PipelineProcess(regs, ram, PC, "data/instr/instructions1.txt", "data/setRegisters.txt", disco, Clock);
}
