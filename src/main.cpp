#include "includes/Opcode.hpp"
#include "includes/Instruction.hpp"
#include "includes/Registers.hpp"
#include "includes/ULA.hpp"
#include "includes/UnidadeControle.hpp"
#include "includes/RAM.hpp"
#include "includes/InstructionDecode.hpp"
#include "includes/Pipeline.hpp"
#include "includes/Core.hpp"
#include "includes/perifericos.hpp"

#include <vector>
#include <iostream>

using namespace std;

int main() {
    RAM ram;
    Disco disco;
    Perifericos periferico;

    // periferico.estadoPeriferico("teclado", true);
    // periferico.estadoPeriferico("mouse", true);
    
    // Criando 2 processos (PCBs) com quantum de 5 ciclos
    vector<PCB> pcbs;
    pcbs.push_back(PCB(1, 5));  // Processo 1 com quantum 5
    pcbs.push_back(PCB(2, 5));  // Processo 2 com quantum 5

    // Inicializando os núcleos com os PCBs
    vector<Core> cores;
    cores.emplace_back(ram, disco, pcbs);  // Passando a lista de PCBs para o núcleo
    cores.emplace_back(ram, disco, pcbs);  // Outro núcleo

    // Rodando os núcleos em threads
    vector<thread> threads;
    for (auto& core : cores) {
        threads.push_back(thread(&Core::run, &core));  // Rodando cada núcleo em uma thread
    }

    // Aguardando que todas as threads terminem
    for (auto& th : threads) {
        th.join();
    }

    // cout << "\nDados RAM\n";
    // ram.display();

    // cout << "\nDados DISCO\n";
    // disco.display();

    // cout << "\nEstado atual da RAM:\n";
    // ram.displayI();
    
    return 0;
}
