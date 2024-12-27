#include "../includes/Bootloader.hpp"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

#define NUM_CORES 1

void Bootloader::inicializarSistema(RAM& ram, Disco& disco, ProcessManager& processManager, vector<unique_ptr<Core>>& cores){
    // Adiciona processos na fila de prontos
    processManager.adicionarProcesso(ProcessControlBlock(1, 0, 5)); // Processo 1
    // processManager.adicionarProcesso(ProcessControlBlock(2, 0, 3)); // Processo 2
    // processManager.adicionarProcesso(ProcessControlBlock(3, 0, 4)); // Processo 3

    // Vetor de núcleos usando unique_ptr
    // vector<unique_ptr<Core>> cores;
    for (int i = 0; i < NUM_CORES; ++i) {
        cores.emplace_back(make_unique<Core>(ram, disco, processManager));
        cores[i]->start(); // Inicia a execução do núcleo
    }

    // Simula tempo de execução
    this_thread::sleep_for(chrono::seconds(10));

    // Para os núcleos
    for (auto& core : cores) {
        core->stop();
    }

    cout << "\nExecução concluída. Todos os núcleos foram parados.\n";

    cout << endl << "Dados DISCO" << endl;
    disco.display();

    cout << endl << "Dados RAM" << endl;
    ram.display();

    cout << endl << "Estado atual da RAM" << endl;
    ram.displayI();
}