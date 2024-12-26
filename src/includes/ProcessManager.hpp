#ifndef PROCESS_MANAGER_HPP
#define PROCESS_MANAGER_HPP

#include "ProcessControlBlock.hpp"
#include <queue>
#include <mutex>
#include <unordered_map>
#include <condition_variable>
#include <string>

class ProcessManager
{
private:
    std::queue<ProcessControlBlock> filaProntos;
    std::queue<ProcessControlBlock> filaBloqueados;
    std::unordered_map<std::string, bool> recursos;
    std::mutex mtx;
    std::condition_variable cv;

public:
    ProcessManager();

    void adicionarProcesso(const ProcessControlBlock &pcb);
    ProcessControlBlock obterProximoProcesso();
    void bloquearProcesso(ProcessControlBlock &pcb, const std::string &recurso);
    void desbloquearProcessos(const std::string &recurso);
    bool temProcessosProntos();
    void setProcessFromFile(const std::string &filename);
};

#endif
