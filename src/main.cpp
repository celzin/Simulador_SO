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

    // Criando processos (PCBs) com quantuns fixos
    vector<PCB*> pcbs;
    pcbs.push_back(new PCB(1, 20, regs));  // Processo 1 com quantum 20
    pcbs.push_back(new PCB(2, 15, regs));  // Processo 2 com quantum 15
    pcbs.push_back(new PCB(3, 10, regs));  // Processo 3 com quantum 10

    // Alocação de memória para cada processo
    for (auto& pcb : pcbs) {
        int enderecoBase = 0 + (pcb->pid - 1) * 5; // Exemplo: faixas de memória de 5 endereços por processo
        int limite = enderecoBase + 4;

        pcb->alocarMemoria(ram, enderecoBase, limite);
    }

    // Adicionando os processos ao escalonador
    for (auto& pcb : pcbs) {
        escalonador.adicionarProcesso(pcb);
    }

    // Criando múltiplos núcleos
    const int NUM_NUCLEOS = 2;
    vector<Core> cores;
    for (int i = 0; i < NUM_NUCLEOS; ++i) {
        cores.push_back(Core(instructionAddress, ram, disco, escalonador)); // Criando núcleos
    }

    // Executando os núcleos em threads
    vector<thread> threads;
    for (auto& core : cores) {
        threads.push_back(thread(&Core::run, &core)); // Rodando cada núcleo em uma thread
    }

    // Thread de desbloqueio de processos
    // thread desbloqueio([&escalonador]() {
    //     while (true) {
    //         this_thread::sleep_for(chrono::seconds(5)); // Simula tempo para desbloqueio
    //         escalonador.desbloquearProcessos();
    //     }
    // });

    // Aguardando que todas as threads terminem
    for (auto& th : threads) {
        th.join(); // Espera todas as threads terminarem
    }

    // Finaliza a thread de desbloqueio
    // desbloqueio.detach();

    // Exibindo o estado final dos processos
    // cout << "\n===== Estado Final dos Processos =====\n";
    // for (auto& pcb : pcbs) {
    //     pcb->exibirPCB();
    // }

    // Exibindo o estado final da RAM
    // cout << "\n===== Estado Final da RAM =====\n";
    // ram.display();

    // Exibindo o estado final do Disco
    // cout << "\n===== Estado Final do Disco =====\n";
    // disco.display();

    // Liberando memória dos processos
    for (auto& pcb : pcbs) {
        pcb->liberarMemoria(ram);
        delete pcb;
    }

    return 0;
}
