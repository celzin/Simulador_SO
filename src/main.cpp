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

    // periferico.estadoPeriferico("teclado", true);
    // periferico.estadoPeriferico("mouse", true);
    
    // Criando 2 processos (PCBs) com quantum de 5 ciclos
    std::vector<PCB*> pcbs;
    pcbs.push_back(new PCB(1, 5));  // Processo 1 com quantum 5
    pcbs.push_back(new PCB(2, 5));  // Processo 2 com quantum 5

    // Adicionando os processos ao escalonador
    for (auto& pcb : pcbs) {
        escalonador.adicionarProcesso(pcb);
    }

    // Criando múltiplos núcleos
    const int NUM_NUCLEOS = 2;  // Definindo o número de núcleos
    std::vector<Core> cores;
    for (int i = 0; i < NUM_NUCLEOS; ++i) {
        cores.push_back(Core(ram, disco, escalonador));  // Criando núcleos
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

    // cout << "\nEstado atual da RAM:\n";
    // ram.displayI();
    
    return 0;
}
