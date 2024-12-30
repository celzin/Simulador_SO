#include "../includes/PCB.hpp"
#include <iostream>
#include <vector>

PCB::PCB(int id, int quantum) : pid(id), estado(PRONTO), PC(0), quantumProcesso(quantum), quantumRestante(quantum) {
    // Inicializa o banco de registradores e a memória do processo
}

void PCB::salvarEstado() {
    // Salva o estado do processo, incluindo o valor dos registradores e PC
    std::cout << "Salvando estado do processo " << pid << " no PCB.\n";
    // Exemplo simplificado (registro de PC e registradores)
}

void PCB::restaurarEstado() {
    // Restaura o estado do processo a partir do PCB
    std::cout << "Restaurando estado do processo " << pid << " no PCB.\n";
    // Exemplo simplificado (restaurando PC e registradores)
}

void PCB::decrementarQuantum() {
    if (quantumRestante > 0) {
        quantumRestante--;
    }
}

bool PCB::quantumExpirado() const {
    return quantumRestante == 0;
}
