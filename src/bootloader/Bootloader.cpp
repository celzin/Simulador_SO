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

void Bootloader::inicializarSistema(Registers& regs, RAM& ram, Disco& disco, vector<Core*>& cores){
    disco.setRegistersFromFile(regs, "data/setRegisters.txt");

    vector<pair<string,int>> processosCarregados = disco.loadProcessFromFiles(ram, "data/instr");
    
    // Gerenciador de processos
    GerenciadorProcessos gerenciador;
    vector<unique_ptr<PCB>> processos;

    const int blocoPorProcesso = 10;

    for (size_t i = 0; i < processosCarregados.size(); ++i) {

        const auto& [file, instructionCount] = processosCarregados[i];

        // Calcula o endereço base para o arquivo atual
        int startAddress = (i == 0) ? 0 : processosCarregados[i - 1].second;

        vector<Instruction> allInstructions;
        for (int j = 0; j < instructionCount; ++j) {
            allInstructions.push_back(ram.fetchInstruction(startAddress + j));
        }

        // Cria o processo com as instruções lidas
        processos.emplace_back(make_unique<PCB>(i + 1, allInstructions, 50, regs));
        gerenciador.adicionarProcesso(processos.back().get());

        // cout << "[DEBUG] Processo criado para o arquivo " << file << " com " << instructionCount << " instruções no endereço base " << startAddress << "." << endl;
    }

    // Criando múltiplos núcleos
    for (int i = 0; i < NUM_CORES; i++) {
        cores.push_back(new Core(i, regs, ram, disco, processosCarregados)); // ID do núcleo
    }

    // Iniciar gerenciador de threads
    GerenciadorThreads gerenciadorThreads(cores, gerenciador);
    gerenciadorThreads.iniciar();

    cout << endl << "Execução concluída. Todos os núcleos foram parados." << endl;

    // Exibir estado final da RAM e do Disco
    cout << endl << "Dados DISCO" << endl;
    disco.display();

    cout << endl << "Dados RAM" << endl;
    ram.display();

    cout << endl << "Estado atual da RAM" << endl;
    ram.displayI();
}