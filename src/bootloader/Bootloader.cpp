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

#define NUM_CORES 1

void Bootloader::inicializarSistema(Registers& regs, RAM& ram, Disco& disco, vector<Core*>& cores){
    disco.setRegistersFromFile(regs, "data/setRegisters.txt");

    int instructionAddress = disco.loadInstructionsFromFile(ram, "data/instr/instructions1.txt");
    if (instructionAddress == -1) {
        cerr << "Erro ao carregar as instruções. Sistema será encerrado." << endl;
        return;
    }
    
    // Criando múltiplos núcleos
    for (int i = 0; i < NUM_CORES; i++) {
        cores.push_back(new Core(i, regs, ram, disco, instructionAddress)); // ID do núcleo
    }

    // Gerenciador de processos
    GerenciadorProcessos gerenciador;

    // Obter todas as intruções da RAM
    vector<Instruction> allInstructions;
    for(int i = 0; i < instructionAddress; i++){
        allInstructions.push_back(ram.fetchInstruction(i));
    }

    // Criando um processo para cada instrução
    vector<unique_ptr<PCB>> processos;
    for(size_t i = 0; i < allInstructions.size(); ++i){
        vector<Instruction> singleInstruction = {allInstructions[i]}; // Apenas uma instrução
        processos.emplace_back(make_unique<PCB>(i+1, singleInstruction, 5, regs)); // ID: i+1, Quantum 5
 
        // Adicionando processos ao gerenciador
        gerenciador.adicionarProcesso(processos.back().get());
        //cout << "[DEBUG] Processo " << (i+1) << " criando com Quantum: 5 e Instrução: " << singleInstruction[0] << endl;

        // cout << "[DEBUG] Processo " << (i+1) << " criado com registradores: ";
        // for(int reg : regs.getAll()){
        //     cout << reg << " ";
        // }
        // cout << endl;
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