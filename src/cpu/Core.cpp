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
        std::cerr << "Nenhum processo atribuído ao Core " << id << "!\n";
        return;
    }

    std::cout << "[Core " << id << "] Iniciando execução do processo " << processoAtual->pid << "\n";

    while (processoAtual->quantum > 0 && PC < processoAtual->instrucoes.size() * 4) {
        Instruction instr = processoAtual->instrucoes[PC / 4];

        DecodedInstruction decoded = InstructionDecode(instr, regs);
        uc.executarInstrucao(regs, ram, PC, disco, Clock);

        processoAtual->quantum--; // Reduz o quantum do processo
        PC += 4; // Avança o PC

        std::cout << "[Core " << id << "] Processo " << processoAtual->pid
                  << " executou instrução com opcode " << instr.op << ".\n";
    }

    // Atualiza o PCB com o estado atual
    processoAtual->PC = PC;
    processoAtual->registradores = regs.getAll();

    std::cout << "[Core " << id << "] Processo " << processoAtual->pid << " finalizado ou preemptado.\n";
}