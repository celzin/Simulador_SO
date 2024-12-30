#include "../includes/Core.hpp"

Core::Core(RAM& ram, Disco& disco) : ram(ram), disco(disco), PC(0), Clock(0) {
    std::cout << "Core inicializado com RAM e Disco.\n";
}

void Core::activate() {
    std::cout << "Iniciando o núcleo...\n";
    // Simulação para um único núcleo (sem threads por enquanto)
    uc.executarInstrucao(regs, ram, PC, disco, Clock);
    std::cout << "PC: " << PC << ", Clock: " << Clock << std::endl;
}

// Função que roda as instruções de um núcleo individualmente
void Core::run() {
    std::cout << "Executando o núcleo (Thread) com PC: " << PC << "\n";
    uc.executarInstrucao(regs, ram, PC, disco, Clock);
}
