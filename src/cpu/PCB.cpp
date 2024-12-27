#include "../includes/PCB.hpp"

// Construtor
ProcessControlBlock::ProcessControlBlock(int id, int page_id, int quantum)
    : process_id(id), state(Ready), pc(0), quantum(quantum), memory_page_id(page_id), timestamp(0) {}

// Alterar estado
void ProcessControlBlock::setState(ProcessState new_state) {
    state = new_state;
}

ProcessState ProcessControlBlock::getState() const {
    return state;
}

// Gerenciar PC
void ProcessControlBlock::updatePC(int new_pc) {
    pc = new_pc;
}

int ProcessControlBlock::getPC() const {
    return pc;
}

// Gerenciar Quantum
void ProcessControlBlock::setQuantum(int new_quantum) {
    quantum = new_quantum;
}

int ProcessControlBlock::getQuantum() const {
    return quantum;
}

// Gerenciar recursos
void ProcessControlBlock::addResource(const std::string& resource) {
    resources.push_back(resource);
}

void ProcessControlBlock::removeResource(const std::string& resource) {
    auto it = std::find(resources.begin(), resources.end(), resource);
    if (it != resources.end()) {
        resources.erase(it); // Use o iterador retornado por std::find
    } else {
        std::cerr << "Recurso não encontrado: " << resource << std::endl;
    }
}
