#ifndef GERENCIADOR_PROCESSOS_HPP
#define GERENCIADOR_PROCESSOS_HPP

#include <queue>
#include <iostream>
#include "PCB.hpp"

class GerenciadorProcessos {
private:
    std::queue<PCB*> filaProntos;    // Processos prontos para execução
    std::queue<PCB*> filaBloqueados; // Processos aguardando desbloqueio

public:
    void adicionarProcesso(PCB* processo);
    PCB* obterProximoProcesso();
    void bloquearProcesso(PCB* processo);
    void desbloquearProcessos(); // Simula desbloqueio periódico
    void mostrarEstados() const; // Exibe os estados das filas
    bool temProcessos() const;
};

#endif
