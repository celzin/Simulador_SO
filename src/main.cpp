#include "includes/PCB.hpp"
#include "includes/GerenciadorProcessos.hpp"
#include "includes/Core.hpp"
#include "includes/GerenciadorThreads.hpp"
#include "includes/Instruction.hpp"
#include "includes/RAM.hpp"
#include "includes/Disco.hpp"

#include <vector>

int main() {
    RAM ram;

    // Processo 1
    int pagina1_proc1 = ram.alocarPagina(1);
    ram.currentProcessId = 1;
    ram.write(pagina1_proc1 * RAM::tamanhoPagina, 100);

    int pagina2_proc1 = ram.alocarPagina(1);
    ram.write(pagina2_proc1 * RAM::tamanhoPagina + 2, 200);

    // Processo 2
    int pagina1_proc2 = ram.alocarPagina(2);
    ram.currentProcessId = 2;
    ram.write(pagina1_proc2 * RAM::tamanhoPagina + 1, 300);

    // Leitura
    ram.currentProcessId = 1;
    std::cout << "Valor Processo 1, Página 1: " 
              << ram.read(pagina1_proc1 * RAM::tamanhoPagina) << std::endl;

    std::cout << "Valor Processo 1, Página 2: " 
              << ram.read(pagina2_proc1 * RAM::tamanhoPagina + 2) << std::endl;

    ram.currentProcessId = 2;
    std::cout << "Valor Processo 2, Página 1: " 
              << ram.read(pagina1_proc2 * RAM::tamanhoPagina + 1) << std::endl;

    // Liberar páginas
    ram.liberarPagina(1);
    ram.liberarPagina(2);

    ram.display();

    return 0;
}

