#include "../includes/Core.hpp"

Core::Core(int instructionAddress, RAM& ram, Disco& disco, Escalonador& escalonador) 
    : instructionAddress(instructionAddress), ram(ram), disco(disco), PC(0), Clock(0), escalonador(escalonador) {}

void Core::activate() {
    while (!escalonador.filaVazia()) {
        PCB* pcb = escalonador.obterProximoProcesso();

        if (pcb == nullptr) {
            std::cerr << "[Núcleo " << std::this_thread::get_id() << "] Erro: Nenhum processo disponível para execução.\n\n";
            return;
        }

        // Pipeline inicializado para restaurar
        std::vector<int> pipelineState;

        pcb->restaurarEstado(pipelineState); // Restaura o estado completo do processo

        std::cout << "[Núcleo " << std::this_thread::get_id() << "] Iniciando execução do processo [PID: " << pcb->pid << "]\n\n";
        pcb->exibirPCB(); // Exibe o estado do PCB restaurado

        while (!pcb->quantumExpirado()) {
            uc.executarInstrucao(instructionAddress, pcb->registradores, ram, pcb->PC, disco, Clock, *pcb);
            pcb->decrementarQuantum();
            std::cout << "[PID: " << pcb->pid << "] Quantum Restante: " << pcb->quantumRestante << "\n\n";
        }

        pcb->salvarEstado(pipeline.getPipelineState()); // Salva o estado completo do processo
        pcb->estado = PRONTO;

        if (pcb->quantumRestante == 0) {
            pcb->estado = FINALIZADO;
            std::cout << "[Núcleo " << std::this_thread::get_id() << "] Processo [PID: " << pcb->pid << "] finalizado.\n\n";
        } else {
            std::cout << "[Núcleo " << std::this_thread::get_id() << "] Quantum expirado para o processo [PID: " << pcb->pid << "]. Retornando à fila.\n\n";
            escalonador.adicionarProcesso(pcb);
        }
    }
}

void Core::run() {
    std::cout << "Executando o núcleo (Thread) com PC: " << PC << "\n\n";
    activate();
}
