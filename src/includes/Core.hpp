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

class Core {
public:
    Registers regs;
    UnidadeControle uc;
    int PC;
    RAM& ram;
    Disco& disco;
    int Clock;

    Core(RAM& ram, Disco& disco);
    void activate();
};

/*
tem que ter uma classe propria pra pipiline com os 5 estagios la dentro, herdando e usando.
classe de registradores para controlar os regs e pc
classe ula tem que fazer a parte do exec da pipeline
classe unidade de controle ela herda a pipeline e a ULA pra gerenciar
A core tem as classes
a uc tem que verificar o que esta fazendo
*/