#include "../includes/Core.hpp"

Core::Core(Registers& regs, RAM& ram, Disco& disco, ProcessManager& pm, int& instructionAddress)
    : regs(regs), ram(ram), disco(disco), processManager(pm), PC(0), Clock(0), instructionAddress(instructionAddress), isRunning(false) {}

void Core::start() {
    isRunning = true;
    coreThread = std::thread(&Core::run, this);
}

void Core::stop() {
    isRunning = false;
    if (coreThread.joinable()) {
        coreThread.join();
    }
}

void Core::run() {
    while (isRunning) {
        if (processManager.temProcessosProntos()) {
            ProcessControlBlock pcb = processManager.obterProximoProcesso();
            pcb.state = EXECUTANDO;
            std::cout << endl << "Core executando processo ID: " << pcb.processID << std::endl;

            executarProcesso(pcb);

            if (pcb.quantum > 0) {
                pcb.saveState(PC, regs);
                pcb.state = PRONTO;
                processManager.adicionarProcesso(pcb);
            } else {
                std::cout << "Processo ID " << pcb.processID << " concluído." << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simula tempo de clock
    }
}

void Core::executarProcesso(ProcessControlBlock& pcb) {
    // Exibe o estado inicial do PCB e registradores
    // std::cout << "\n[Core::executarProcesso] Iniciando execução do processo ID: " << pcb.processID << std::endl;
    // std::cout << "Estado inicial dos registradores (do PCB):" << std::endl;
    // pcb.regs.display(); // Exibe os registradores salvos no PCB
    
    PC = pcb.PC;
    regs = pcb.regs;

    while (pcb.quantum > 0 && isRunning) {
        uc.executarInstrucao(regs, ram, PC, disco, Clock, instructionAddress);
        pcb.quantum--;
        std::cout << "Quantum restante para processo " << pcb.processID << ": " << pcb.quantum << std::endl;
    }
}
