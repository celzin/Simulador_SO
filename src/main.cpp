#include "includes/PCB.hpp"
#include "includes/GerenciadorProcessos.hpp"
#include "includes/Core.hpp"
#include "includes/GerenciadorThreads.hpp"
#include "includes/Instruction.hpp"
#include "includes/RAM.hpp"
#include "includes/Disco.hpp"

#include <vector>

int main() {
    // Inicialização da RAM e Core
    RAM ram;
    Disco disco;
    Core core1(0, ram, disco);

    // Criação de processos e PCBs
    std::vector<Instruction> instrsProcesso1 = {
        Instruction(ADD, 1, 2, 3),
        Instruction(SUB, 4, 5, 6),
        Instruction(LOAD, 7, 8, 0)
    };

    std::vector<Instruction> instrsProcesso2 = {
        Instruction(ADD, 1, 2, 3),
        Instruction(STORE, 4, 5, 0)
    };

    PCB processo1(1, instrsProcesso1, 5); // PID 1, Quantum 5
    PCB processo2(2, instrsProcesso2, 5); // PID 2, Quantum 5

    // Alocar páginas para os processos
    processo1.tabelaPaginas[0] = ram.alocarPagina(processo1.pid); // Página lógica 0
    processo1.tabelaPaginas[1] = ram.alocarPagina(processo1.pid); // Página lógica 1
    std::cout << "[Core 0] Páginas alocadas para o processo 1\n";

    processo2.tabelaPaginas[0] = ram.alocarPagina(processo2.pid); // Página lógica 0
    std::cout << "[Core 0] Páginas alocadas para o processo 2\n";

    // Atribuir processos ao Core
    core1.setProcesso(&processo1);
    core1.setProcesso(&processo2);

    // Exibir estado inicial dos PCBs
    processo1.displayPCB();
    processo2.displayPCB();

    // Testar operações de escrita e leitura no processo 1
    ram.currentProcessId = processo1.pid;
    ram.write(0, 42); // Escrever na primeira página lógica
    ram.write(5, 99); // Escrever na segunda página lógica

    int valor1 = ram.read(0); // Ler da primeira página lógica
    int valor2 = ram.read(5); // Ler da segunda página lógica

    std::cout << "Valor lido do processo 1, página lógica 0: " << valor1 << std::endl;
    std::cout << "Valor lido do processo 1, página lógica 1: " << valor2 << std::endl;

    // Testar operações de escrita e leitura no processo 2
    ram.currentProcessId = processo2.pid;
    ram.write(4, 123); // Escrever na primeira página lógica
    int valor3 = ram.read(4); // Ler da primeira página lógica

    std::cout << "Valor lido do processo 2, página lógica 1: " << valor3 << std::endl;

    // Exibir estado final da RAM
    ram.display();

    return 0;
}

