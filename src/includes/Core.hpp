#ifndef CORE_HPP
#define CORE_HPP

#include "Disco.hpp"
#include "Escalonador.hpp"
#include "Instruction.hpp"
#include "PCB.hpp"
#include "Pipeline.hpp"
#include "RAM.hpp"
#include "Registers.hpp"
#include "UnidadeControle.hpp"

#include <thread>
#include <vector>
#include <sstream>
#include <filesystem>

using namespace std;

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

    Core(RAM& ram, Disco& disco, Escalonador& escalonador);
    void activate(ofstream& outfile);  
    void run();   

private:
    void validateMemoryAccess(PCB* processo, int endereco, ofstream& outfile);
    void gerenciarRecursos(PCB* processo, ofstream& outfile);  
};

#endif