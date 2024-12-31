#include "../includes/UnidadeControle.hpp"

void UnidadeControle::executarInstrucao(Registers& regs, RAM& ram, int& PC, Disco& disco, int& Clock, PCB& pcb) {
    // Verifica se o quantum expirou
    if (pcb.quantumExpirado()) {
        std::cout << "Quantum expirado para o processo " << pcb.pid << ". Troca de contexto.\n";
        pcb.salvarEstado(pipeline.getPipelineState()); // Salva o estado completo
        return;
    }

    // Pipeline inicializado para restaurar
    std::vector<int> pipelineState;
    pcb.restaurarEstado(pipelineState); // Restaura o estado completo do processo
    PC = pcb.PC; // Atualiza o PC do processo
    std::cout << "Executando processo " << pcb.pid << " no PC: " << PC << "\n";
    pcb.decrementarQuantum();
    // Executa o pipeline do processo
    pipeline.PipelineProcess(regs, ram, PC, "data/instructions.txt", "data/setRegisters.txt", disco, Clock);
    pcb.PC = PC; // Atualiza o estado do PCB após a execução
    pcb.salvarEstado(pipeline.getPipelineState()); // Atualiza o PCB com o novo estado
}