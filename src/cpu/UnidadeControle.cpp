#include "../includes/UnidadeControle.hpp"

void UnidadeControle::executarInstrucao(const Instruction& instr, Registers& regs, RAM& ram, int& PC, Disco& disco, int& Clock, PCB& pcb, ofstream& outfile) {
    // Verifica se o quantum expirou
    if (pcb.quantumExpirado()) {
        std::cout << "Quantum expirado para o processo " << pcb.pid << ". Troca de contexto.\n";
        pcb.salvarEstado(pipeline.getPipelineState()); // Salva o estado completo
        return;
    }

    // Pipeline inicializado para restaurar
    std::vector<int> pipelineState;
    pcb.restaurarEstado(pipelineState, outfile); // Restaura o estado completo do processo
    PC = pcb.PC; // Atualiza o PC do processo
    // std::cout << "Executando processo " << pcb.pid << " no PC: " << PC << "\n";
    pcb.decrementarQuantum(outfile);
    // Executa o pipeline do processo
    pipeline.PipelineProcess(instr, regs, ram, PC, disco, Clock);
    pcb.PC = PC; // Atualiza o estado do PCB após a execução
    pcb.salvarEstado(pipeline.getPipelineState()); // Atualiza o PCB com o novo estado
}