#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#include "PCB.hpp"
#include <queue>
#include <vector>
#include <mutex>

class Escalonador {
private:
    std::queue<PCB*> filaProntos;
    std::queue<PCB*> filaBloqueados;
    std::mutex mtx;

public:
    Escalonador(); 
    void adicionarProcesso(PCB* processo);  // Adiciona um processo à fila
    PCB* obterProximoProcesso();  // Retorna o próximo processo a ser executado

    void bloquearProcesso(PCB* processo);
    void desbloquearProcessos(); 
    bool filaVazia() const;  
};

#endif
