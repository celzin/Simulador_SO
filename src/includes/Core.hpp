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

#include <thread>
#include <atomic>

class Core {
public:
    Registers regs;
    UnidadeControle uc;
    int PC;
    RAM& ram;
    Disco& disco;
    int Clock;

    Core(RAM& ram, Disco& disco);
    void activate(); // Método principal de execução do core
    void run();      // Método executado dentro da thread
    void start();    // Método para iniciar a thread
    void stop();     // Método para parar a thread

private:
    std::thread coreThread;      // Thread associada ao núcleo
    std::atomic<bool> isRunning; // Controle do estado de execução
};

#endif
