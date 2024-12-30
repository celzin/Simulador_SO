#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#include "PCB.hpp"
#include <queue>
#include <vector>

class Escalonador {
private:
    std::queue<PCB*> filaProcessos;  // Fila de processos prontos

public:
    Escalonador();  // Construtor que inicializa a fila
    void adicionarProcesso(PCB* processo);  // Adiciona um processo à fila
    PCB* obterProximoProcesso();  // Retorna o próximo processo a ser executado

    bool filaVazia() const;  // Verifica se a fila está vazia
};

#endif