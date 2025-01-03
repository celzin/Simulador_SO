#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#include "PCB.hpp"
#include <queue>
#include <vector>
#include <mutex>

using namespace std;

class Escalonador {
private:
    std::queue<PCB*> filaProntos;
    std::queue<PCB*> filaBloqueados;
    std::mutex mtx;

public:
    Escalonador(); 
    void adicionarProcesso(PCB* processo);  // Adiciona um processo à fila
    PCB* obterProximoProcesso(ofstream& outfile);  // Retorna o próximo processo a ser executado

    void bloquearProcesso(PCB* processo, ofstream& outfile);
    void desbloquearProcessos(ofstream& outfile); 
    bool filaVazia() const;  
};

#endif
