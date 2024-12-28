#include "../includes/UnidadeControle.hpp"

void UnidadeControle::executarInstrucao(Registers& regs, RAM& ram, int& PC, Disco& disco, int& Clock, int instructionAddress) {
        pipeline.PipelineProcess(regs, ram, PC, disco, Clock, instructionAddress);
}
