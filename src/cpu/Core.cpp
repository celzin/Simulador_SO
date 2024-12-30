#include "../includes/Core.hpp"

Core::Core(RAM& ram, Disco& disco, std::vector<PCB>& pcbs) 
    : ram(ram), disco(disco), PC(0), Clock(0), processos(pcbs) {}

void Core::activate() {
    if (processos.empty()) {
        std::cout << "Nenhum processo para executar.\n";
        return;
    }

    // Aqui, pegamos o primeiro processo da lista (apenas como exemplo)
    PCB& pcb = processos[0];
    pcb.estado = EXECUCAO;
    pcb.salvarEstado();

    while (!pcb.quantumExpirado()) {
        std::cout << "Executando processo " << pcb.pid << ", PC: " << pcb.PC << "\n";
        // Simulação de execução do processo, alterando o PC
        pcb.PC += 4;  // Exemplo simplificado

        // Decrementa o quantum restante
        pcb.decrementarQuantum();
        Clock++;
    }

    // Quantum expirado, realizar a troca de contexto
    pcb.estado = FINALIZADO;
    pcb.restaurarEstado();
    std::cout << "Processo " << pcb.pid << " finalizado.\n";
}

// Função que roda as instruções de um núcleo individualmente
void Core::run() {
    std::cout << "Executando o núcleo (Thread) com PC: " << PC << "\n";
    
    // Aqui, vamos acessar o PCB do processo a ser executado pelo núcleo
    // Supondo que o núcleo está executando o primeiro processo da lista de processos
    if (!processos.empty()) {
        PCB& pcb = processos[0];  // Pegamos o primeiro processo da lista de PCBs
        uc.executarInstrucao(regs, ram, PC, disco, Clock, pcb);  // Passamos o PCB para a função
    } else {
        std::cerr << "Nenhum processo disponível para execução.\n";
    }
}
