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
    int pagina1 = ram.alocarPagina(1); // Página lógica 0 do Processo 1
    ram.currentProcessId = 1;
    ram.write(pagina1 * RAM::tamanhoPagina, 42); // Escreve na página lógica 0

    // Processo 2
    int pagina2 = ram.alocarPagina(2); // Página lógica 0 do Processo 2
    ram.currentProcessId = 2;
    ram.write(pagina2 * RAM::tamanhoPagina + 1, 99); // Escreve na página lógica 0 com offset 1

    // Leitura
    ram.currentProcessId = 1;
    int valor1 = ram.read(pagina1 * RAM::tamanhoPagina);

    ram.currentProcessId = 2;
    int valor2 = ram.read(pagina2 * RAM::tamanhoPagina + 1);

    std::cout << "Valor 1 lido: " << valor1 << std::endl; // Deve ser 42
    std::cout << "Valor 2 lido: " << valor2 << std::endl; // Deve ser 99

    return 0;
}
