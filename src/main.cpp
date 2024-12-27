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
#include "includes/ProcessManager.hpp"

#include <vector>
#include <iostream>
#include <thread>
#include <memory>

#define NUM_CORES 1

using namespace std;

int main() {
    RAM ram;
    Disco disco;
    ProcessManager processManager;

    // Adiciona processos na fila de prontos
    processManager.adicionarProcesso(ProcessControlBlock(1, 0, 5)); // Processo 1
    // processManager.adicionarProcesso(ProcessControlBlock(2, 0, 3)); // Processo 2
    // processManager.adicionarProcesso(ProcessControlBlock(3, 0, 4)); // Processo 3

    // Vetor de núcleos usando std::unique_ptr
    std::vector<std::unique_ptr<Core>> cores;

    for (int i = 0; i < NUM_CORES; ++i) {
        cores.emplace_back(std::make_unique<Core>(ram, disco, processManager));
        cores[i]->start(); // Inicia a execução do núcleo
    }

    // Simula tempo de execução
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // Para os núcleos
    for (auto& core : cores) {
        core->stop();
    }

    std::cout << "\nExecução concluída. Todos os núcleos foram parados.\n";

    cout << endl << "Dados DISCO" << endl;
    disco.display();

    cout << endl << "Dados RAM" << endl;
    ram.display();

    cout << endl << "Estado atual da RAM" << endl;
    ram.displayI();

    return 0;
}
