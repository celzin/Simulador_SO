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

    periferico.estadoPeriferico("teclado", true);
    periferico.estadoPeriferico("mouse", true);
    
    // Criar múltiplos núcleos (cores)
    vector<Core> cores;
    int numCores = 2;  // Pode ajustar esse número para mais núcleos

    // Inicializa os núcleos
    for (int i = 0; i < numCores; i++) {
        cout << "Inicializando Core " << i << endl;
        cores.emplace_back(ram, disco);
    }

    // Ativa os núcleos em paralelo usando threads
    vector<thread> threads;
    for (int i = 0; i < numCores; i++) {
        threads.push_back(thread(&Core::run, &cores[i]));
    }

    // Espera todos os núcleos terminarem sua execução
    for (auto& th : threads) {
        th.join();
    }

    cout << "Execução MAIN finalizada!\n";

    // cout << "\nDados RAM\n";
    // ram.display();

    // cout << "\nDados DISCO\n";
    // disco.display();

    // cout << "\nEstado atual da RAM:\n";
    // ram.displayI();
    
    return 0;
}
