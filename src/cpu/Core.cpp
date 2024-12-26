#include "../includes/Core.hpp"
#include <mutex>
#include <thread>
#include <iostream>

std::mutex coutMutex;

Core::Core(RAM &ram, Disco &disco, ProcessManager &pm)
    : ram(ram), disco(disco), processManager(pm), PC(0), Clock(0), isRunning(false) {}

void Core::start()
{
    isRunning = true;
    coreThread = std::thread(&Core::run, this);
    std::cout << "[Core] Núcleo iniciado. Thread ID: " << coreThread.get_id() << "\n";
}

void Core::stop()
{
    isRunning = false;
    if (coreThread.joinable())
    {
        coreThread.join();
    }
    std::cout << "[Core] Núcleo finalizado. Thread ID: " << coreThread.get_id() << "\n";
}

void Core::run()
{
    while (isRunning)
    {
        if (processManager.temProcessosProntos())
        {
            ProcessControlBlock pcb = processManager.obterProximoProcesso();
            pcb.state = EXECUTANDO;

            {
                std::lock_guard<std::mutex> lock(coutMutex);
                std::cout << "[Core] Executando processo ID: " << pcb.processID
                          << " com Quantum: " << pcb.quantum << "\n";
            }

            executarProcesso(pcb);

            if (pcb.state == PRONTO && pcb.quantum > 0)
            {
                pcb.saveState(PC, regs);
                processManager.adicionarProcesso(pcb);
            }
            else if (pcb.state == BLOQUEADO)
            {
                processManager.bloquearProcesso(pcb, pcb.resource);
            }
            else
            {
                std::lock_guard<std::mutex> lock(coutMutex);
                std::cout << "[Core] Processo ID " << pcb.processID << " concluído.\n";
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Core::executarProcesso(ProcessControlBlock &pcb)
{
    PC = pcb.PC;
    regs = pcb.regs;

    while (pcb.quantum > 0 && isRunning)
    {
        // uc.executarInstrucao(regs, ram, PC, disco, Clock);
        pcb.quantum--;

        {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cout << "[Core] Processo ID " << pcb.processID
                      << " executando. Quantum restante: " << pcb.quantum << "\n";
        }

        // Simula bloqueio -> Remover pos testes
        if (pcb.quantum == 2)
        {
            pcb.state = BLOQUEADO;
            pcb.resource = "teclado";
            return;
        }
    }
}
