#include "../includes/UnidadeControle.hpp"

void UnidadeControle::executarInstrucao(Registers& regs, RAM& ram, int& PC, Disco& disco, int& Clock, PCB& pcb) {
    // Verifica se o processo tem quantum restante
    if (pcb.quantumExpirado()) {
        cout << "Quantum expirado para o processo " << pcb.pid << ". Troca de contexto.\n";
        // Troca de contexto: salvar o estado do processo atual
        pcb.salvarEstado();
        // A lógica de troca de contexto irá aqui, por exemplo, escolher outro PCB
        return;  // A execução do processo será interrompida e outro será escolhido
    }

    PC = pcb.PC;  // Utilizando o PC do processo
    cout << "Executando processo " << pcb.pid << " no PC: " << PC << endl;

    pcb.decrementarQuantum();  // Decrementa o quantum restante

    // Executa a instrução no pipeline ou faz qualquer outro trabalho necessário
    pipeline.PipelineProcess(regs, ram, PC, "data/instructions.txt", "data/setRegisters.txt", disco, Clock);

    // Atualiza o estado do processo no PCB
    pcb.restaurarEstado();
}
