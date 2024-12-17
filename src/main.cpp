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
#include <thread>
#include <memory>

using namespace std;

int main() {
    RAM ram;
    Disco disco;
    vector<std::unique_ptr<Core>> cores;
    Perifericos periferico;

    periferico.estadoPeriferico("teclado", true);
    periferico.estadoPeriferico("mouse", true);

    for (int i = 0; i < 2; i++) {
        std::cout << "CORE " << i << ": " << std::endl;
        cores.emplace_back(std::make_unique<Core>(ram, disco));
        cores[i]->start(); // Inicia a thread do núcleo
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));

    for (auto& core : cores) {
        core->stop();
    }

    std::cout << "\nDados RAM\n";
    ram.display();

    std::cout << "\nDados DISCO\n";
    disco.display();

    std::cout << "\nEstado atual da RAM:\n";
    ram.displayI();

    return 0;
}
