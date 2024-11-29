#include "../includes/Core.hpp"

Core::Core(RAM& ram, Disco& disco) : ram(ram), disco(disco), PC(0), Clock(0) {
    cout << "Core inicializado com RAM\n";
}

void Core::activate() {
    //PipelineProcess(regs, ram, uc, PC, "data/instructions.txt", "data/setRegisters.txt", disco);
    uc.executarInstrucao(regs,ram,PC,disco,Clock);
    cout << "PC: " << PC << endl;
}