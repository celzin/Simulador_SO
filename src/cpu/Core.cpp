#include "../includes/Core.hpp"

Core::Core(RAM& ram, Disco& disco, Escalonador& escalonador) 
    : ram(ram), disco(disco), PC(0), Clock(0), escalonador(escalonador) {}

void Core::activate() {
    while (!escalonador.filaVazia()) {
        PCB* pcb = escalonador.obterProximoProcesso();

        if (pcb == nullptr) {
            std::cerr << "Erro: Nenhum processo disponível para execução.\n";
            return;
        }

        // Pipeline inicializado para restaurar
        std::vector<int> pipelineState;

        pcb->restaurarEstado(pipelineState); // Restaura o estado completo do processo
        pcb->exibirPCB(); // Exibe o estado do PCB restaurado

        while (!pcb->quantumExpirado()) {
            uc.executarInstrucao(pcb->registradores, ram, pcb->PC, disco, Clock, *pcb);
            pcb->decrementarQuantum();
        }

        pcb->salvarEstado(pipeline.getPipelineState()); // Salva o estado completo do processo
        pcb->estado = PRONTO;
        pcb->exibirPCB(); // Exibe o estado do PCB após execução

        if (pcb->quantumRestante == 0) {
            pcb->estado = FINALIZADO;
            std::cout << "Processo " << pcb->pid << " finalizado.\n";
        } else {
            escalonador.adicionarProcesso(pcb);
        }
    }
}

void Core::run() {
    std::cout << "Executando o núcleo (Thread) com PC: " << PC << "\n";
    activate();
}
