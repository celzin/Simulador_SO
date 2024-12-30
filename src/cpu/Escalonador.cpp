#include "../includes/Escalonador.hpp"

Escalonador::Escalonador() {}

PCB* Escalonador::obterProximoProcesso() {
    if (!filaVazia()) {
        PCB* processo = filaProcessos.front();
        filaProcessos.pop();
        std::cout << "Retirando o processo " << processo->pid << " da fila de execução.\n";
        return processo;  // Retorna o primeiro processo da fila
    }
    std::cerr << "Erro: A fila de processos está vazia!\n";
    return nullptr;  // Retorna nullptr se a fila estiver vazia
}

void Escalonador::adicionarProcesso(PCB* processo) {
    filaProcessos.push(processo);  // Adiciona o processo à fila de prontos
}

bool Escalonador::filaVazia() const {
    return filaProcessos.empty();
}
