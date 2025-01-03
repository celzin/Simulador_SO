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
#include <fstream>

#define NUM_NUCLEOS 4
#define OUTPUT_DIR "data/logs"

using namespace std;

int main() {
    RAM ram;
    Disco disco;
    Perifericos periferico;
    Escalonador escalonador;
    Registers regs;

    // periferico.estadoPeriferico("teclado", true);
    // periferico.estadoPeriferico("mouse", true);
    
    // Configurando os registradores
    disco.setRegistersFromFile(regs, "data/setRegisters.txt");

    // Lista de arquivos de instrução
    vector<string> arquivosInstrucoes = disco.listInstructionsFile("data/instr");

    // Criando PCBs
    vector<PCB*> pcbs = ProcessManager::createPCBs(disco, ram, regs, arquivosInstrucoes);

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
    vector<Core> cores;
    for (int i = 0; i < NUM_NUCLEOS; ++i) {
        cores.push_back(Core(ram, disco, escalonador)); // Criando núcleos
    }

    // Executando os núcleos em threads
    vector<thread> threads;
    for (auto& core : cores) {
        threads.push_back(thread(&Core::run, &core)); // Rodando cada núcleo em uma thread
    }

    // Aguardando que todas as threads terminem
    for (auto& th : threads) {
        th.join(); // Espera todas as threads terminarem
    }

    // // Exibindo o estado final da RAM
    // cout << "\n===== Estado Final da RAM =====\n";
    // ram.display();

    // // Exibindo o estado final do Disco
    // cout << "\n===== Estado Final do Disco =====\n";
    // disco.display();

    // Liberando memória dos processos
    for (auto& pcb : pcbs) {
        pcb->liberarMemoria(ram);
        delete pcb;
    }

    return 0;
}
