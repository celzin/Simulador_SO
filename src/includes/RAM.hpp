#ifndef RAM_HPP
#define RAM_HPP

#include "Instruction.hpp"
#include "InstructionDecode.hpp"

#include <iostream>
#include <vector>
#include <mutex>
#include <stdexcept>

using namespace std;

class RAM {
private:
    mutex mtx;
    int totalQuadros;
    vector<bool> quadrosUsados;
    vector<int> memoria;
    vector<Instruction> instruction_memory;
    int size;

public:
    static const int TAMANHO_QUADRO = 4;

    // Apenas declaração do construtor
    RAM(int tamanho);

    int getSize() const { return size; }

    // Métodos de gerenciamento de quadros
    int alocarQuadro();
    void liberarQuadro(int indiceQuadro);
    void escreverNoQuadro(int indiceQuadro, const vector<int>& dados);
    vector<int> lerQuadro(int indiceQuadro);

    // Operações na memória de dados
    void write(int endereco, int valor);
    int read(int endereco);

    // Operações na memória de instruções
    void writeInstruction(int endereco, const Instruction& instr);
    Instruction fetchInstruction(int endereco) const;

    // Métodos auxiliares
    bool isReserved(int endereco) const;
    void display(ofstream& outfile) const;
    void displayI() const;

    // Acesso à memória de instruções (apenas leitura)
    const vector<Instruction>& getInstructionMemory() const { return instruction_memory; }
};

#endif
