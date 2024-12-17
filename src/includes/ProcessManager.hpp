#ifndef PROCESS_MANAGER_HPP
#define PROCESS_MANAGER_HPP

#include "ProcessControlBlock.hpp"
#include <queue>
#include <mutex>
#include <condition_variable>

class ProcessManager {
private:
    std::queue<ProcessControlBlock> filaProntos;
    std::queue<ProcessControlBlock> filaBloqueados;
    std::mutex mtx;
    std::condition_variable cv;

public:
    void adicionarProcesso(const ProcessControlBlock& pcb);
    ProcessControlBlock obterProximoProcesso();
    void bloquearProcesso(ProcessControlBlock& pcb, const std::string& recurso);
    void desbloquearProcessos(const std::string& recurso);
    bool temProcessosProntos();
};

#endif
