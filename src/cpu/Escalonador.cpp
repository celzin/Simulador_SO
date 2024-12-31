#include "../includes/Escalonador.hpp"

Escalonador::Escalonador() {}

PCB* Escalonador::obterProximoProcesso() {
    std::lock_guard<std::mutex> lock(mtx);
    if (!filaVazia()) {
        PCB* processo = filaProntos.front();
        filaProntos.pop();
        std::cout << "Retirando o processo " << processo->pid << " da fila de execução.\n";
        return processo;  // Retorna o primeiro processo da fila
    }
    std::cerr << "Erro: A fila de processos está vazia!\n";
    return nullptr;  // Retorna nullptr se a fila estiver vazia
}

void Escalonador::adicionarProcesso(PCB* processo) {
    std::lock_guard<std::mutex> lock(mtx);
    filaProntos.push(processo);
}

void Escalonador::bloquearProcesso(PCB* processo) {
    std::lock_guard<std::mutex> lock(mtx);
    processo->atualizarEstado(BLOQUEADO);
    filaBloqueados.push(processo);
    std::cout << "[Escalonador] Processo " << processo->pid << " movido para a fila de bloqueados.\n";
}

void Escalonador::desbloquearProcessos() {
    std::lock_guard<std::mutex> lock(mtx);
    while (!filaBloqueados.empty()) {
        PCB* processo = filaBloqueados.front();
        filaBloqueados.pop();
        processo->atualizarEstado(PRONTO);
        filaProntos.push(processo);
        std::cout << "[Escalonador] Processo " << processo->pid << " desbloqueado e movido para a fila de prontos.\n";
    }
}

bool Escalonador::filaVazia() const {
    return filaProntos.empty();
}