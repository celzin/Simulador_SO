#ifndef PCB_HPP
#define PCB_HPP

#include <vector>
#include <string>
#include "Instruction.hpp"

// Estados de processo
enum ProcessState {
    PRONTO,
    BLOQUEADO,
    EXECUTANDO
};

class PCB {
public:
    int pid;                         // Identificador único do processo
    ProcessState estado;             // Estado atual do processo
    int PC;                          // Contador de programa
    int quantum;                     // Quantum restante
    std::vector<int> registradores;  // Registradores do processo
    std::vector<Instruction> instrucoes; // Instruções carregadas na memória

    PCB(int id, const std::vector<Instruction>& instrs, int quantumInicial);
};

#endif