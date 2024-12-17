#include "../includes/Core.hpp"

Core::Core(RAM& ram, Disco& disco)
    : ram(ram), disco(disco), PC(0), Clock(0), isRunning(false) {
    std::cout << "Core inicializado com RAM\n";
}

void Core::activate() {
    // Método principal chamado pela thread
    uc.executarInstrucao(regs, ram, PC, disco, Clock);
    std::cout << "PC: " << PC << std::endl;
}

void Core::run() {
    // Método executado pela thread do núcleo
    while (isRunning) {
        activate(); // Executa o pipeline ou outras tarefas do core
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simula execução
    }
}

void Core::start() {
    // Inicia a thread associada ao núcleo
    isRunning = true;
    coreThread = std::thread(&Core::run, this); // Vincula o método run à thread
}

void Core::stop() {
    // Para a thread associada ao núcleo
    isRunning = false;
    if (coreThread.joinable()) {
        coreThread.join(); // Aguarda a finalização segura da thread
    }
}