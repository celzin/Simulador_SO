#ifndef RAM_HPP
#define RAM_HPP

#include <iostream>
#include <vector>
#include <unordered_map>
#include "Instruction.hpp"

class RAM {
private:
    std::unordered_map<int, int> tabelaPaginas; // Mapeamento de endereços lógicos -> físicos
    std::vector<bool> paginasOcupadas;         // Controle de páginas ocupadas

public:
    static const int tamanho = 12;             // Tamanho total da memória
    static const int tamanhoPagina = 4;        // Tamanho de cada página

    std::vector<int> memoria;
    std::vector<Instruction> instruction_memory;

    RAM();

    void write(int endereco, int valor);
    int read(int endereco);

    void writeInstruction(int endereco, const Instruction& instr);
    Instruction fetchInstruction(int endereco) const;

    int alocarPagina(int processoId);
    void liberarPagina(int processoId);
    void display() const;
    void displayI() const;
};

#endif
