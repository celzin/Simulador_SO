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
#include "includes/PCB.hpp"

#include <vector>
#include <iostream>

using namespace std;

// Exemplo de geração de PCBs para teste
std::vector<ProcessControlBlock> createTestPCBs() {
    std::vector<ProcessControlBlock> pcbs;
    pcbs.emplace_back(1, 100, 30);
    pcbs.emplace_back(2, 101, 25);
    pcbs.emplace_back(3, 102, 20);
    return pcbs;
}

int main() {
    RAM ram;
    Disco disco;
    std::vector<Core> cores;
    Perifericos periferico;

    periferico.estadoPeriferico("teclado", true);
    periferico.estadoPeriferico("mouse", true);

    for (int i = 0; i < 2; i++) {
        cores.emplace_back(ram, disco);
    }

    // Criando PCBs de teste
    auto pcbs = createTestPCBs();

    // Carregando processos nos núcleos e ativando
    for (size_t i = 0; i < cores.size(); ++i) {
        if (i < pcbs.size()) {
            cores[i].loadProcess(&pcbs[i]);
            cores[i].activate();
        }
    }

    // Exibindo estado final da RAM e do disco
    std::cout << "\nDados RAM\n";
    ram.display();

    std::cout << "\nDados DISCO\n";
    disco.display();

    std::cout << "\nEstado atual da RAM:\n";
    ram.displayI();

    return 0;
}













