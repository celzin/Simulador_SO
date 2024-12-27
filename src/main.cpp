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
#include "includes/PCB.hpp"

#include <vector>
#include <iostream>

using namespace std;

int main() {
    // Criando três PCBs simulados
    ProcessControlBlock pcb1(1, 100, 30);
    ProcessControlBlock pcb2(2, 101, 25);
    ProcessControlBlock pcb3(3, 102, 20);

    // Exibindo informações iniciais
    std::cout << "PCB1 - ID: " << pcb1.process_id << ", Estado: " << pcb1.getState() << ", Quantum: " << pcb1.getQuantum() << std::endl;
    std::cout << "PCB2 - ID: " << pcb2.process_id << ", Estado: " << pcb2.getState() << ", Quantum: " << pcb2.getQuantum() << std::endl;

    // Alterando estados e PC
    pcb1.setState(Running);
    pcb1.updatePC(10);
    pcb2.setState(Blocked);

    std::cout << "PCB1 - Novo Estado: " << pcb1.getState() << ", Novo PC: " << pcb1.getPC() << std::endl;
    std::cout << "PCB2 - Novo Estado: " << pcb2.getState() << std::endl;

    // Adicionando e removendo recursos
    pcb1.addResource("arquivo1.txt");
    pcb1.addResource("arquivo2.txt");
    std::cout << "PCB1 - Recursos após adições: ";
    for (const auto& res : pcb1.resources) {
        std::cout << res << " ";
    }
    std::cout << std::endl;

    pcb1.removeResource("arquivo1.txt");
    std::cout << "PCB1 - Recursos após remoção: ";
    for (const auto& res : pcb1.resources) {
        std::cout << res << " ";
    }
    std::cout << std::endl;

    // Testando remoção de recurso inexistente
    pcb1.removeResource("recurso_inexistente");

    return 0;
}













