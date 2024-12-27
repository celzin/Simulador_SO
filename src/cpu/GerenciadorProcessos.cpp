#include "../includes/GerenciadorProcessos.hpp"

void GerenciadorProcessos::adicionarProcesso(PCB* processo) {
    filaProntos.push(processo);
    std::cout << "Processo " << processo->pid << " adicionado à fila de prontos." << std::endl;
}

PCB* GerenciadorProcessos::obterProximoProcesso() {
    if (filaProntos.empty()) {
        std::cerr << "Erro: Nenhum processo na fila de prontos!" << std::endl;
        return nullptr;
    }
    PCB* processo = filaProntos.front();
    filaProntos.pop();
    return processo;
}

void GerenciadorProcessos::bloquearProcesso(PCB* processo) {
    processo->estado = BLOQUEADO;
    filaBloqueados.push(processo);
    std::cout << "Processo " << processo->pid << " movido para a fila de bloqueados." << std::endl;
}

void GerenciadorProcessos::desbloquearProcessos() {
    while (!filaBloqueados.empty()) {
        PCB* processo = filaBloqueados.front();
        filaBloqueados.pop();
        processo->estado = PRONTO;
        filaProntos.push(processo);
        std::cout << "Processo " << processo->pid << " desbloqueado e movido para a fila de prontos." << std::endl;
    }
}

void GerenciadorProcessos::mostrarEstados() const {
    std::cout << "Fila de Prontos: ";
    std::queue<PCB*> copiaFilaProntos = filaProntos;
    while (!copiaFilaProntos.empty()) {
        PCB* processo = copiaFilaProntos.front();
        copiaFilaProntos.pop();
        std::cout << processo->pid << " ";
    }
    std::cout << std::endl;

    std::cout << "Fila de Bloqueados: ";
    std::queue<PCB*> copiaFilaBloqueados = filaBloqueados;
    while (!copiaFilaBloqueados.empty()) {
        PCB* processo = copiaFilaBloqueados.front();
        copiaFilaBloqueados.pop();
        std::cout << processo->pid << " ";
    }
    std::cout << std::endl;
}
