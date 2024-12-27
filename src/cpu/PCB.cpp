#include "../includes/PCB.hpp"

PCB::PCB(int id, const std::vector<Instruction>& instrs, int quantumInicial)
    : pid(id), estado(PRONTO), PC(0), quantum(quantumInicial), registradores(12, 0), instrucoes(instrs) {}
