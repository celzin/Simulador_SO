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
#include "includes/GerenciadorProcessos.hpp"

#include <vector>
#include <iostream>

using namespace std;

int main() {
    // Criação do gerenciador de processos
    GerenciadorProcessos gerenciador;

    // Criação de instruções fictícias
    std::vector<Instruction> instr1 = {Instruction(ADD, 0, 1, 2), Instruction(SUB, 1, 0, 3)};
    std::vector<Instruction> instr2 = {Instruction(MULT, 2, 3, 4), Instruction(DIV, 4, 3, 2)};

    // Criação de processos
    PCB processo1(1, instr1, 5); // ID: 1, Quantum: 5
    PCB processo2(2, instr2, 5); // ID: 2, Quantum: 5

    // Adicionar processos ao gerenciador
    gerenciador.adicionarProcesso(&processo1);
    gerenciador.adicionarProcesso(&processo2);

    // Mostrar estados iniciais
    gerenciador.mostrarEstados();

    // Bloquear um processo e mostrar estados
    gerenciador.bloquearProcesso(&processo1);
    gerenciador.mostrarEstados();

    // Desbloquear processos e mostrar estados
    gerenciador.desbloquearProcessos();
    gerenciador.mostrarEstados();

    return 0;
}
