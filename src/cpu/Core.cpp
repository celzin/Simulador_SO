#include "../includes/Core.hpp"

Core::Core(RAM& ram, Disco& disco, Escalonador& escalonador) 
    : ram(ram), disco(disco), PC(0), Clock(0), escalonador(escalonador) {}

void Core::activate() {
    std::cout << "Iniciando o núcleo...\n";

    while (!escalonador.filaVazia()) {
        PCB* pcb = escalonador.obterProximoProcesso();

        if (pcb == nullptr) {
            std::cerr << "Erro: Nenhum processo disponível para execução.\n";
            return;
        }

        std::cout << "Iniciando execução do processo " << pcb->pid << " com quantum " << pcb->quantumProcesso << "\n";

        // Executa o processo até o quantum expirar
        while (!pcb->quantumExpirado()) {
            std::cout << "Quantum restante para o processo " << pcb->pid << ": " << pcb->quantumRestante << "\n";
            uc.executarInstrucao(regs, ram, PC, disco, Clock, *pcb);
            pcb->decrementarQuantum();  // Decrementa o quantum
        }

        std::cout << "Quantum expirado para o processo " << pcb->pid << ". Realizando troca de contexto.\n";

        // Salva o estado e verifica se o processo precisa ser finalizado ou re-adicionado à fila
        pcb->salvarEstado();

        // Marca como FINALIZADO se o processo não tiver mais instruções a executar
        if (pcb->quantumRestante == 0) {
            pcb->estado = FINALIZADO;
            std::cout << "Processo " << pcb->pid << " finalizado.\n";
        } else {
            std::cout << "Recolocando o processo " << pcb->pid << " de volta na fila.\n";
            escalonador.adicionarProcesso(pcb);
        }
    }
}

void Core::run() {
    std::cout << "Executando o núcleo (Thread) com PC: " << PC << "\n";
    activate();
}
