#include "../includes/Core.hpp"

Core::Core(int id, Registers& regs, RAM& ram, Disco& disco, int& instructionAddress) 
    : id(id), regs(regs), ram(ram), disco(disco), PC(0), Clock(0), instructionAddress(instructionAddress), processoAtual(nullptr) {
        std::cout << std::endl << "Core " << id << " inicializado com RAM e Disco\n";
}

void Core::setProcesso(PCB* processo) {
    processoAtual = processo;
    PC = processo->PC;

    // Atualizar cada registrador individualmente usando 'set'
    const auto& registradoresPCB = processo->registradores;
    for(size_t i = 0; i < registradoresPCB.size(); ++i){
        regs.set(i, registradoresPCB[i]);
    }

    std::cout << "Processo " << processo->pid << " atribuído ao Core " << id << ".\n";
}

void Core::executarProcesso() {
    if (processoAtual == nullptr) {
        std::cerr << "[Core " << id << "] Nenhum processo atribuído!\n";
        return;
    }

    // Exibir o PCB antes da execução
    std::cout << "[Core " << id << "] Informações do processo antes da execução:\n";
    processoAtual->displayPCB();
    // std::cout << "Estado inicial dos registradores:\n";
    // regs.display();

    // Execução do processo
    while (processoAtual->quantum > 0 && PC < processoAtual->instrucoes.size() * 4) {
        Instruction instr = processoAtual->instrucoes[PC / 4];
        DecodedInstruction decoded = InstructionDecode(instr, regs);
        uc.executarInstrucao(regs, ram, PC, disco, Clock, instructionAddress);

        processoAtual->quantum--;
        PC += 4;
    }

    // Atualizar PCB com o estado final
    processoAtual->PC = PC;
    processoAtual->registradores = regs.getAll();

    // Exibir o PCB após a execução
    std::cout << "[Core " << id << "] Informações do processo após a execução:\n";
    // regs.display();
    processoAtual->displayPCB();
}
