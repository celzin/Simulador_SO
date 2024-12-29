#include "../includes/Bootloader.hpp"
#include "../includes/PCB.hpp"
#include "../includes/GerenciadorProcessos.hpp"
#include "../includes/Core.hpp"
#include "../includes/GerenciadorThreads.hpp"
#include "../includes/Instruction.hpp"
#include "../includes/RAM.hpp"
#include "../includes/Disco.hpp"

#include <vector>
#include <iostream>
#include <thread>

using namespace std;

#define NUM_CORES 2

void Bootloader::inicializarSistema(Registers& regs, RAM& ram, Disco& disco, std::vector<Core*>& cores) {
    disco.setRegistersFromFile(regs, "data/setRegisters.txt");

    std::vector<std::pair<std::string, int>> processosCarregados = disco.loadProcessFromFiles(ram, "data/instr");

    // Gerenciador de processos
    GerenciadorProcessos gerenciador;
    std::vector<std::unique_ptr<PCB>> processos;

    for (size_t i = 0; i < processosCarregados.size(); ++i) {
        const auto& [file, quantumProcesso] = processosCarregados[i];
        std::vector<Instruction> allInstructions;

        int startAddress = (i == 0) ? 0 : processosCarregados[i - 1].second;
        for (int j = 0; j < quantumProcesso; ++j) {
            allInstructions.push_back(ram.fetchInstruction(startAddress + j));
        }

        processos.emplace_back(std::make_unique<PCB>(i + 1, allInstructions, quantumProcesso, regs));
        gerenciador.adicionarProcesso(processos.back().get());
    }

    // Criar núcleos com quantums independentes
    for (int i = 0; i < NUM_CORES; i++) {
        cores.push_back(new Core(i, regs, ram, disco, processosCarregados, 30)); // Quantum do núcleo definido como 30
    }

    // Iniciar gerenciador de threads
    GerenciadorThreads gerenciadorThreads(cores, gerenciador);
    gerenciadorThreads.iniciar();

    cout << endl << "Execução concluída. Todos os núcleos foram parados." << endl;

    // Exibir estado final da RAM e do Disco
    // cout << endl << "Dados DISCO" << endl;
    // disco.display();

    // cout << endl << "Dados RAM" << endl;
    // ram.display();

    // cout << endl << "Estado atual da RAM" << endl;
    // ram.displayI();
}