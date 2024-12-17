#include "../includes/ProcessManager.hpp"

void ProcessManager::adicionarProcesso(const ProcessControlBlock& pcb) {
    std::lock_guard<std::mutex> lock(mtx);
    filaProntos.push(pcb);
}

ProcessControlBlock ProcessManager::obterProximoProcesso() {
    std::lock_guard<std::mutex> lock(mtx);
    if (!filaProntos.empty()) {
        ProcessControlBlock pcb = filaProntos.front();
        filaProntos.pop();
        return pcb;
    }
    return ProcessControlBlock(-1, 0, 0); // Nenhum processo disponível
}

void ProcessManager::bloquearProcesso(ProcessControlBlock& pcb, const std::string& recurso) {
    std::lock_guard<std::mutex> lock(mtx);
    pcb.state = BLOQUEADO;
    pcb.resource = recurso;
    filaBloqueados.push(pcb);
}

void ProcessManager::desbloquearProcessos(const std::string& recurso) {
    std::lock_guard<std::mutex> lock(mtx);
    std::queue<ProcessControlBlock> tempQueue;
    while (!filaBloqueados.empty()) {
        ProcessControlBlock pcb = filaBloqueados.front();
        filaBloqueados.pop();
        if (pcb.resource == recurso) {
            pcb.state = PRONTO;
            filaProntos.push(pcb);
        } else {
            tempQueue.push(pcb);
        }
    }
    filaBloqueados = tempQueue;
}

bool ProcessManager::temProcessosProntos() {
    std::lock_guard<std::mutex> lock(mtx);
    return !filaProntos.empty();
}
