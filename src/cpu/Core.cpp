#include "../includes/Core.hpp"

Core::Core(int id, RAM& ram, Disco& disco) : id(id), ram(ram), disco(disco), PC(0), Clock(0), processoAtual(nullptr) {
    std::cout << "Core " << id << " inicializado com RAM e Disco\n";
}

void Core::setProcesso(PCB* processo) {
    processoAtual = processo;
    PC = processo->PC;
    regs = Registers(); // Reinicializa os registradores
    std::cout << "Processo " << processo->pid << " atribuído ao Core " << id << ".\n";
}

void Core::executarProcesso() {
    if (processoAtual == nullptr) {
        std::cerr << "[Core " << id << "] Nenhum processo atribuído!\n";
        return;
    }

    std::cout << "[Core " << id << "] Iniciando execução do processo " << processoAtual->pid << ".\n";

    std::cout << "Estado inicial dos registradores:\n";
    regs.display();

    while (processoAtual->quantum > 0 && PC < processoAtual->instrucoes.size() * 4) {
        Instruction instr = processoAtual->instrucoes[PC / 4];

        DecodedInstruction decoded = InstructionDecode(instr, regs);
        uc.executarInstrucao(regs, ram, PC, disco, Clock);

        processoAtual->quantum--; // Reduz o quantum do processo
        PC += 4; // Avança o PC
    }

    // Atualiza o PCB com o estado atual
    processoAtual->PC = PC;
    processoAtual->registradores = regs.getAll();

    std::cout << "[Core " << id << "] Estado final dos registradores para o processo " << processoAtual->pid << ":\n";
    regs.display();
}
