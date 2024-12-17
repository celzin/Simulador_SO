#ifndef CORE_HPP
#define CORE_HPP

#include "Opcode.hpp"
#include "Instruction.hpp"
#include "Registers.hpp"
#include "ULA.hpp"
#include "UnidadeControle.hpp"
#include "RAM.hpp"
#include "InstructionDecode.hpp"
#include "Pipeline.hpp"
#include "Disco.hpp"
#include "Pipeline.hpp"
#include "ProcessManager.hpp"
#include "ProcessControlBlock.hpp"

#include <thread>
#include <atomic>

class Core {
public:
    Registers regs;
    UnidadeControle uc;
    int PC;
    RAM& ram;
    Disco& disco;
    ProcessManager& processManager;
    int Clock;

    Core(RAM& ram, Disco& disco, ProcessManager& pm);
    void start();     // Inicia a thread do Core
    void stop();      // Para a thread do Core
    void run();       // Executa a lógica de execução (loop do core)

private:
    std::thread coreThread;
    std::atomic<bool> isRunning;

    void executarProcesso(ProcessControlBlock& pcb);
};

#endif
