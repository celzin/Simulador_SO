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
    RAM ram;
    Disco disco;

    // Criando um único núcleo
    Core core(ram, disco);

    // Gerenciador de processos
    GerenciadorProcessos gerenciador;

    // Criando instruções fictícias
    std::vector<Instruction> instr1 = {Instruction(ADD, 0, 1, 2), Instruction(SUB, 1, 0, 3)};
    std::vector<Instruction> instr2 = {Instruction(MULT, 2, 3, 4), Instruction(DIV, 4, 3, 2)};

    // Criando processos
    PCB processo1(1, instr1, 5); // ID: 1, Quantum: 5
    PCB processo2(2, instr2, 5); // ID: 2, Quantum: 5

    // Adicionando processos ao gerenciador
    gerenciador.adicionarProcesso(&processo1);
    gerenciador.adicionarProcesso(&processo2);

    // Executando processos manualmente no núcleo
    PCB* processoAtual = gerenciador.obterProximoProcesso();
    core.setProcesso(processoAtual);
    core.executarProcesso();

    processoAtual = gerenciador.obterProximoProcesso();
    core.setProcesso(processoAtual);
    core.executarProcesso();

    return 0;
}
