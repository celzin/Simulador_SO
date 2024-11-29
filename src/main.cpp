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
    vector<Core> cores;
    Perifericos periferico;

    periferico.estadoPeriferico("teclado", true);
    periferico.estadoPeriferico("mouse", true);

    for (int i = 0; i < 2; i++)
    {   cout << "CORE " << i << ": " << endl;
        cores.emplace_back(ram, disco);
        cout << endl << endl << endl;
    }

    cout << "Utilizando o Core 0:" << endl;
    cores[0].activate();

    cout << "\nDados RAM\n";
    ram.display();

    cout << "\nDados DISCO\n";
    disco.display();

    cout << "\nEstado atual da RAM:\n";
    ram.displayI();
    

    return 0;
}













