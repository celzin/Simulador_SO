#ifndef PCB_HPP
#define PCB_HPP

#include "Registers.hpp"
#include <vector>

enum EstadoProcesso {
    PRONTO,        // Processo está pronto para execução
    EXECUCAO,      // Processo está sendo executado
    BLOQUEADO,     // Processo está aguardando algum recurso
    FINALIZADO     // Processo terminou sua execução
};

class PCB {
public:
    int pid;                          // ID do processo
    EstadoProcesso estado;            // Estado atual do processo
    int PC;                            // Contador de programa
    Registers registradores;          // Banco de registradores
    int quantumProcesso;              // Quantum de processo (tempo máximo de execução)
    int quantumRestante;              // Quantum restante para o processo
    std::vector<int> memoriaProcesso; // Memória alocada ao processo (endereço base e limites)
    
    PCB(int id, int quantum);
    void salvarEstado();
    void restaurarEstado();
    void decrementarQuantum();
    bool quantumExpirado() const;
};

#endif
