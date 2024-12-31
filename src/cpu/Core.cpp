#include "../includes/Core.hpp"

Core::Core(int instructionAddress, RAM& ram, Disco& disco, Escalonador& escalonador) 
    : instructionAddress(instructionAddress), ram(ram), disco(disco), PC(0), Clock(0), escalonador(escalonador) {}

void Core::activate() {
    while (!escalonador.filaVazia()) {
        PCB* pcb = escalonador.obterProximoProcesso();

        if (pcb == nullptr) {
            std::cerr << "[Núcleo " << std::this_thread::get_id() << "] Erro: Nenhum processo disponível para execução.\n";
            return;
        }

        // Restaurar o estado do processo
        std::vector<int> pipelineState;
        pcb->restaurarEstado(pipelineState);

        pcb->atualizarEstado(EXECUCAO);
        std::cout << "[Núcleo " << std::this_thread::get_id() << "] Iniciando execução do processo [PID: " << pcb->pid << "]\n";
        pcb->exibirPCB(); // Imprime o estado inicial do PCB

        while (!pcb->quantumExpirado()) {
            // Gerenciar recursos
            gerenciarRecursos(pcb);

            // Executa uma instrução no pipeline
            uc.executarInstrucao(instructionAddress, pcb->registradores, ram, pcb->PC, disco, Clock, *pcb);
            pcb->decrementarQuantum();

            if (pcb->verificarEstado(BLOQUEADO)) {
                break;
            }
        }

        // Salvar o estado do processo
        pcb->salvarEstado(pipeline.getPipelineState());

        std::cout << "[Núcleo " << std::this_thread::get_id() << "] Finalizando execução do processo [PID: " << pcb->pid << "]:\n";
        pcb->exibirPCB(); // Exibe o estado final do PCB

        if (pcb->quantumRestante == 0) {
            pcb->atualizarEstado(FINALIZADO);
            std::cout << "[Núcleo " << std::this_thread::get_id() << "] Processo [PID: " << pcb->pid << "] finalizado.\n";
        } else if (pcb->verificarEstado(BLOQUEADO)) {
            std::cout << "[Núcleo " << std::this_thread::get_id() << "] Processo [PID: " << pcb->pid << "] bloqueado.\n";
        } else {
            pcb->atualizarEstado(PRONTO);
            escalonador.adicionarProcesso(pcb);
            std::cout << "[Núcleo " << std::this_thread::get_id() << "] Quantum expirado para o processo [PID: " << pcb->pid << "]. Retornando à fila de prontos.\n";
        }
    }
}

void Core::run() {
    std::cout << "Executando o núcleo (Thread) com PC: " << PC << "\n\n";
    activate();
}

void Core::gerenciarRecursos(PCB* processo) {
    // Simula uma verificação de recursos
    std::string recursoNecessario = "teclado"; // Exemplo de recurso
    if (!processo->verificarRecurso(recursoNecessario)) {
        processo->associarRecurso(recursoNecessario, true); // Aloca o recurso
    } else {
        processo->atualizarEstado(BLOQUEADO); // Bloqueia o processo caso o recurso esteja indisponível
    }
}
