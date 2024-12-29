#include "../includes/Core.hpp"

Core::Core(int id, Registers& regs, RAM& ram, Disco& disco, std::vector<std::pair<std::string, int>>& processosCarregados, int quantumNucleo)
    : id(id), regs(regs), ram(ram), disco(disco), PC(0), Clock(0), processosCarregados(processosCarregados), processoAtual(nullptr), quantumNucleo(quantumNucleo) {
    std::cout << "Core " << id << " inicializado com RAM e Disco.\n";
}

void Core::setProcesso(PCB* processo) {
    processoAtual = processo;
    PC = processo->PC;

    // Atualizar cada registrador individualmente usando 'set'
    const auto& registradoresPCB = processo->registradores;
    for (size_t i = 0; i < registradoresPCB.size(); ++i) {
        regs.set(i, registradoresPCB[i]);
    }

    std::cout << "Processo " << processo->pid << " atribuído ao Core " << id << ".\n";
}

void Core::executarProcesso() {
    if (processoAtual == nullptr) {
        std::cerr << "[Core " << id << "] Nenhum processo atribuído!\n";
        return;
    }

    int quantumRestanteNucleo = quantumNucleo; // Quantum disponível no núcleo para este ciclo

    // Exibir o PCB antes da execução
    std::cout << "[Core " << id << "] Informações do processo antes da execução:\n";
    processoAtual->displayPCB();

    // Execução do processo
    while (processoAtual->quantum > 0 && quantumRestanteNucleo > 0 && PC < processoAtual->instrucoes.size() * 4) {
        Instruction instr = processoAtual->instrucoes[PC / 4];
        std::cout << "[Core " << id << "] Executando instrução no PC=" << PC
                  << ": Opcode=" << instr.op << std::endl;

        DecodedInstruction decoded = InstructionDecode(instr, regs);
        uc.executarInstrucao(regs, ram, PC, disco, Clock, processoAtual->instrucoes.size());

        std::cout << "[PREEMP][Core " << id << "] Quantum do núcleo restante: " << quantumRestanteNucleo
          << ", Quantum do processo restante: " << processoAtual->quantum
          << ", PC: " << PC << "\n";

        processoAtual->quantum--;         // Decrementar quantum do processo
        quantumRestanteNucleo--;         // Decrementar quantum do núcleo
        PC += 4;                         // Avançar PC
    }

    // Atualizar PCB com estado final
    processoAtual->PC = PC;
    processoAtual->registradores = regs.getAll();

    if (processoAtual->quantum > 0) {
        std::cout << "[PREEMP][Core " << id << "] Preempção forçada: Quantum do núcleo esgotado.\n";
        std::cout << "[PREEMP] Quantum restante do processo: " << processoAtual->quantum << "\n";
        std::cout << "[PREEMP] PC salvo no PCB: " << PC << "\n";
    }

    // Exibir o PCB após a execução
    std::cout << "[Core " << id << "] Informações do processo após a execução:\n";
    processoAtual->displayPCB();
}