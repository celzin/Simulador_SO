#ifndef RAM_HPP
#define RAM_HPP

#include <iostream>
#include <vector>
#include <unordered_map>
#include "Instruction.hpp"
#include <unordered_map>

struct hash_pair {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

class RAM {
private:
    std::unordered_map<std::pair<int, int>, int, hash_pair> tabelaPaginas;
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

    int currentProcessId;
    int alocarPagina(int processoId);
    void liberarPagina(int processoId);
    void display() const;
    void displayI() const;
};

#endif
