#include "../includes/PCB.hpp"
#include <iostream>
#include <iomanip>

PCB::PCB(int id, int quantum, const Registers& regs)
    : pid(id), estado(PRONTO), PC(0), quantumProcesso(quantum), quantumRestante(quantum), registradores(regs) {}

void PCB::atualizarEstado(EstadoProcesso novoEstado) {
    estado = novoEstado;
    std::cout << "[PCB] Processo " << pid << " alterado para estado: "
              << (novoEstado == PRONTO ? "PRONTO" : novoEstado == EXECUCAO ? "EXECUCAO" : novoEstado == BLOQUEADO ? "BLOQUEADO" : "FINALIZADO") << std::endl;
}

bool PCB::verificarEstado(EstadoProcesso verEstado) const {
    return estado == verEstado;
}

void PCB::salvarEstado(const std::vector<int>& pipelineState) {
    // Salva o estado do pipeline
    estadoPipeline = pipelineState;

    // Salva os registradores no PCB
    std::vector<std::pair<int, bool>> registradoresAtuais = registradores.getAllRegisters();
    registradores.setAllRegisters(registradoresAtuais);

    // Salva o valor atual do PC
    this->PC = PC;

    std::cout << "Estado completo do processo " << pid << " salvo no PCB.\n";
}

void PCB::restaurarEstado(std::vector<int>& pipelineState) {
    // Restaura o estado do pipeline
    pipelineState = estadoPipeline;

    // Restaura os registradores do PCB
    const std::vector<std::pair<int, bool>>& estadoRegistradores = registradores.getAllRegisters();
    registradores.setAllRegisters(estadoRegistradores);

    // Restaura o valor do PC
    this->PC = PC;

    std::cout << "Estado completo do processo " << pid << " restaurado do PCB.\n";
}

void PCB::decrementarQuantum() {
    if (quantumRestante > 0) {
        quantumRestante--;
    }
}

bool PCB::quantumExpirado() const {
    return quantumRestante <= 0;
}

void PCB::alocarMemoria(int enderecoBase, int limite) {
    memoriaAlocada = {enderecoBase, limite};
    std::cout << "Memória alocada ao processo " << pid << ": Base=" << enderecoBase << ", Limite=" << limite << "\n";
}

bool PCB::verificarAcessoMemoria(int endereco) const {
    if (memoriaAlocada.empty()) return false;
    return endereco >= memoriaAlocada[0] && endereco <= memoriaAlocada[1];
}

void PCB::liberarMemoria() {
    memoriaAlocada.clear();
    std::cout << "Memória liberada para o processo " << pid << "\n";
}

// Gerenciamento de recursos (I/O)
void PCB::associarRecurso(const std::string& nomeRecurso, bool alocado) {
    recursos.estadoPeriferico(nomeRecurso, alocado);
    std::cout << "Recurso " << nomeRecurso << (alocado ? " alocado" : " liberado") << " ao processo " << pid << ".\n";
}

bool PCB::verificarRecurso(const std::string& nomeRecurso) const {
    return recursos.verificarPeriferico(nomeRecurso);
}

void PCB::exibirPCB() const {
    std::cout << "\n===============================" << "\n"
              << "[PCB] Processo ID: " << pid << "\n"
              << "Estado: " << (estado == PRONTO ? "PRONTO" : estado == EXECUCAO ? "EXECUCAO" : estado == BLOQUEADO ? "BLOQUEADO" : "FINALIZADO") << "\n"
              << "Quantum Total: " << quantumProcesso << ", Quantum Restante: " << quantumRestante << "\n"
              << "PC: " << PC << "\n"
              << "Registradores:\n";
    registradores.display();

    std::cout << "Memória Alocada:\n";
    if (!memoriaAlocada.empty()) {
        std::cout << "  Base: " << memoriaAlocada[0] << ", Limite: " << memoriaAlocada[1] << "\n";
    } else {
        std::cout << "  Nenhuma memória alocada\n";
    }

    std::cout << "Estado do Pipeline: ";
    if (!estadoPipeline.empty()) {
        for (const auto& val : estadoPipeline) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    } else {
        std::cout << "  Nenhum estado salvo\n";
    }

    std::cout << "Recursos Associados:\n";
    recursos.exibirPerifericos();

    std::cout << "===============================\n\n";
}
