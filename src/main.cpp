#include "includes/PCB.hpp"
#include "includes/GerenciadorProcessos.hpp"
#include "includes/Core.hpp"
#include "includes/GerenciadorThreads.hpp"
#include "includes/Instruction.hpp"
#include "includes/RAM.hpp"
#include "includes/Disco.hpp"

#include <vector>

int main() {
    RAM ram;
    Disco disco;

    // Criando múltiplos núcleos
    std::vector<Core*> cores;
    for (int i = 0; i < 2; i++) { // 2 núcleos
        cores.push_back(new Core(ram, disco));
    }

    // Gerenciador de processos
    GerenciadorProcessos gerenciador;

    // Criando instruções fictícias
    std::vector<Instruction> instr1 = {
        Instruction(ADD, 1, 2, 3), Instruction(STORE, 3, 1, 0), Instruction(LOAD, 0, 1, 0)};
    std::vector<Instruction> instr2 = {
        Instruction(MULT, 4, 2, 3), Instruction(DIV, 5, 3, 2), Instruction(STORE, 2, 4, 0)};
    std::vector<Instruction> instr3 = {
        Instruction(AND, 0, 1, 2), Instruction(OR, 3, 4, 5), Instruction(LOAD, 0, 2, 0)};

    // Criando processos
    PCB processo1(1, instr1, 5); // ID: 1, Quantum: 5
    PCB processo2(2, instr2, 5); // ID: 2, Quantum: 5
    PCB processo3(3, instr3, 5); // ID: 3, Quantum: 5

    // Adicionando processos ao gerenciador
    gerenciador.adicionarProcesso(&processo1);
    gerenciador.adicionarProcesso(&processo2);
    gerenciador.adicionarProcesso(&processo3);

    // Iniciar gerenciador de threads
    GerenciadorThreads gerenciadorThreads(cores, gerenciador);
    gerenciadorThreads.iniciar();

    // Exibir estado final da RAM e do Disco
    std::cout << "\nEstado final da RAM:\n";
    ram.display();

    std::cout << "\nEstado final do Disco:\n";
    disco.display();

    return 0;
}
