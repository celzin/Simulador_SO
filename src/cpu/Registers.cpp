#include "../includes/Registers.hpp"

Registers::Registers() : registradores(NUM_REGISTRADORES, make_pair(0, false)) {}

std::vector<int> Registers::getAll() const {
    std::vector<int> valores;
    for (const auto& reg : registradores) {
        valores.push_back(reg.first); // Adiciona o valor de cada registrador
    }
    return valores;
}

int Registers::get(int index) const {
    if (index >= 0 && index < registradores.size()) {
        return registradores[index].first;
    }
    cerr << "Erro: Registrador inválido " << index << endl;
    return 0;
}

void Registers::set(int index, int value) {
    if (index >= 0 && index < registradores.size()) {
        if (registradores[index].first != value) {
            registradores[index] = {value, true};
        } else {
            registradores[index].second = false;
        }
    } else {
        cerr << "Erro: Registrador inválido " << index << endl;
    }
}


void Registers::display() const {
    cout << "\nRegistradores:\n";
    for (size_t i = 0; i < registradores.size(); ++i) {
        cout << "R[" << i << "] = " << registradores[i].first
             << ", Flag = " << (registradores[i].second ? "sujo" : "limpo") << endl;
    }
}
