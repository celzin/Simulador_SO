#include "../includes/PCB.hpp"

PCB::PCB(int id, const std::vector<Instruction>& instrs, int quantumInicial)
    : pid(id), estado(PRONTO), PC(0), quantum(quantumInicial), registradores(12, 0), instrucoes(instrs) {}

void PCB::displayPCB() const {
    std::cout << "=== PCB do Processo " << pid << " ===" << std::endl;
    std::cout << "Estado: ";
    switch (estado) {
        case PRONTO: std::cout << "Pronto"; break;
        case BLOQUEADO: std::cout << "Bloqueado"; break;
        case EXECUTANDO: std::cout << "Executando"; break;
    }
    std::cout << std::endl;
    std::cout << "PC: " << PC << std::endl;
    std::cout << "Quantum Restante: " << quantum << std::endl;

    std::cout << "Registradores:" << std::endl;
    for (size_t i = 0; i < registradores.size(); ++i) {
        std::cout << "R[" << i << "] = " << registradores[i] << std::endl;
    }

    std::cout << "Instruções Carregadas (limite: 5 primeiras):" << std::endl;
    for (size_t i = 0; i < std::min(instrucoes.size(), static_cast<size_t>(5)); ++i) {
        std::cout << "[" << i << "] Opcode: " << instrucoes[i].op
                  << ", Destino: R" << instrucoes[i].Destiny_Register
                  << ", R1: " << instrucoes[i].Register_1
                  << ", R2: " << instrucoes[i].Register_2 << std::endl;
    }
    std::cout << "==========================" << std::endl;
}