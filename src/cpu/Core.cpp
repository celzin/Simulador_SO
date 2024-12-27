#include "../includes/Core.hpp"

Core::Core(RAM& ram, Disco& disco) : ram(ram), disco(disco), PC(0), Clock(0), currentProcess(nullptr) {
    std::cout << "Core inicializado com RAM\n";
}

void Core::loadProcess(ProcessControlBlock* pcb) {
    currentProcess = pcb;
    PC = pcb->getPC(); // Sincroniza o PC com o processo
    std::cout << "Processo " << pcb->process_id << " carregado no Core.\n";
}

void Core::activate() {
    if (currentProcess == nullptr) {
        std::cout << "Nenhum processo carregado no Core.\n";
        return;
    }

    // uc.executarInstrucao(regs, ram, PC, disco, Clock);
    currentProcess->updatePC(PC); // Atualiza o PCB com o PC atual

    // Simulação de término de execução para teste
    currentProcess->setState(Ready); // Exemplo de alternância de estado
    std::cout << "Processo " << currentProcess->process_id << " concluído no Core.\n";
}
