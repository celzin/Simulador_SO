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
#include "includes/ProcessManager.hpp"

#include <vector>
#include <iostream>
#include <thread>
#include <memory>

#define NUM_CORES 2

using namespace std;

int main() {
    RAM ram;
    Disco disco;
    ProcessManager processManager;

    // Adiciona processos na fila de prontos
    processManager.setProcessFromFile("data/setProcess.txt");

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

    return 0;
}
