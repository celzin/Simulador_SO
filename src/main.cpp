#include "includes/Opcode.hpp"
#include "includes/Instruction.hpp"
#include "includes/Registers.hpp"
#include "includes/ULA.hpp"
#include "includes/UnidadeControle.hpp"
#include "includes/RAM.hpp"
#include "includes/InstructionDecode.hpp"
#include "includes/Pipeline.hpp"
#include "includes/Core.hpp"
#include "includes/Perifericos.hpp"

#include <vector>
#include <iostream>

using namespace std;

int main() {
    RAM ram;
    Disco disco;
    Perifericos periferico;
    Escalonador escalonador;
    Registers regs;

    // periferico.estadoPeriferico("teclado", true);
    // periferico.estadoPeriferico("mouse", true);
    
    // Configurando os registradores e instruções
    disco.setRegistersFromFile(regs, "data/setRegisters.txt");
    int instructionAddress = disco.loadInstructionsFromFile(ram, "data/instr/instructions1.txt");
    if (instructionAddress == -1) {
        cerr << "Erro ao carregar as instruções.\n";
        return -1;
    }

    // Criando 3 processos (PCBs) com quantuns fixos
    std::vector<PCB*> pcbs;
    pcbs.push_back(new PCB(1, 20, regs));  // Processo 1 com quantum 20
    pcbs.push_back(new PCB(2, 15, regs));  // Processo 2 com quantum 15
    pcbs.push_back(new PCB(3, 10, regs));  // Processo 3 com quantum 10

    // Adicionando os processos ao escalonador
    for (auto& pcb : pcbs) {
        escalonador.adicionarProcesso(pcb);
    }

    // Criando múltiplos núcleos
    const int NUM_NUCLEOS = 2;
    std::vector<Core> cores;
    for (int i = 0; i < NUM_NUCLEOS; ++i) {
        cores.push_back(Core(instructionAddress, ram, disco, escalonador));
    }

    // Executando os núcleos em threads
    std::vector<std::thread> threads;
    for (auto& core : cores) {
        threads.push_back(std::thread(&Core::run, &core));  // Rodando cada núcleo em uma thread
    }

    // Aguardando que todas as threads terminem
    for (auto& th : threads) {
        th.join();  // Espera todas as threads terminarem
    }
    
    // cout << "\nDados RAM\n";
    // ram.display();

    // cout << "\nDados DISCO\n";
    // disco.display();

    // Limpando memória
    // for (auto& pcb : pcbs) {
    //     delete pcb;
    // }
    
    return 0;
}
