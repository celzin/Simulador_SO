#include "../includes/Core.hpp"

Core::Core(RAM& ram, Disco& disco, ProcessManager& pm)
    : ram(ram), disco(disco), processManager(pm), PC(0), Clock(0), isRunning(false) {}

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
            std::cout << "Core executando processo ID: " << pcb.processID << std::endl;

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
    PC = pcb.PC;
    regs = pcb.regs;

    while (pcb.quantum > 0 && isRunning) {
        uc.executarInstrucao(regs, ram, PC, disco, Clock);
        pcb.quantum--;
        std::cout << "Quantum restante para processo " << pcb.processID << ": " << pcb.quantum << std::endl;
    }
}
