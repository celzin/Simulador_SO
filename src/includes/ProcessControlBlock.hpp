#ifndef PROCESS_CONTROL_BLOCK_HPP
#define PROCESS_CONTROL_BLOCK_HPP

#include "Registers.hpp"
#include <string>

enum ProcessState {
    PRONTO,
    EXECUTANDO,
    BLOQUEADO
};

class ProcessControlBlock {
public:
    int processID;                 // Identificador único do processo
    ProcessState state;            // Estado do processo
    int PC;                        // Contador de Programa
    int quantum;                   // Tempo restante até preempção
    Registers regs;                // Estado dos registradores
    std::string resource;          // Recurso aguardado (se bloqueado)

    ProcessControlBlock(int id, int pc, int q)
        : processID(id), state(PRONTO), PC(pc), quantum(q), resource("") {}

    void saveState(int pc, const Registers& registers);
    void restoreState(int& pc, Registers& registers);
};

#endif
