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

    // Exibir o estado inicial do PCB
    std::cout << "Estado inicial do PCB:\n";
    currentProcess->display();

    uc.executarInstrucao(regs, ram, PC, disco, Clock);

    // Atualizar o PCB após a execução
    currentProcess->updatePC(PC);
    currentProcess->setState(Ready); // Simulação de troca de estado

    // Exibir o estado final do PCB
    std::cout << "Estado final do PCB após execução:\n";
    currentProcess->display();

    std::cout << "Processo " << currentProcess->process_id << " concluído no Core.\n";
}
