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
#include "Escalonador.hpp"

#include <thread>
#include <vector>

class Core {
public:
    int PC;                         
    int Clock;         
    int instructionAddress;

    Registers regs;                
    UnidadeControle uc;           
    RAM& ram;               
    Disco& disco;        
    Escalonador& escalonador;    
    Pipeline pipeline; 

    Core(int instructionAddress, RAM& ram, Disco& disco, Escalonador& escalonador);
    void activate();  
    void run();   

private:
    void gerenciarRecursos(PCB* processo);  
};

#endif