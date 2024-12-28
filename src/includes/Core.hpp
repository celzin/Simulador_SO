#ifndef CORE_HPP
#define CORE_HPP

#include "Opcode.hpp"
#include "Instruction.hpp"
#include "Registers.hpp"
#include "ULA.hpp"
#include "UnidadeControle.hpp"
#include "RAM.hpp"
#include "InstructionDecode.hpp"
#include "Pipeline.hpp"
#include "Disco.hpp"
#include "Pipeline.hpp"
#include "PCB.hpp"

class Core {
public:
    int id; // Identificador único do núcleo
    Registers regs;
    UnidadeControle uc;
    int PC;
    RAM& ram;
    Disco& disco;
    int Clock;
    PCB* processoAtual; // PCB do processo em execução

    Core(int id, RAM& ram, Disco& disco);
    void executarProcesso(); // Executa o processo atual
    void setProcesso(PCB* processo); // Configura o processo a ser executado
    void alocarPaginas(PCB& processo); // Método para alocar páginas para o processo
};

#endif