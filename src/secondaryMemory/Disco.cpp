#include "../includes/Disco.hpp"

Disco::Disco() : memoria(LINHAS, std::vector<std::pair<int, bool>>(COLUNAS, {0, false})) {}

void Disco::write(int valor) {
    std::lock_guard<std::mutex> lock(mtx);  // Bloqueia o mutex enquanto a função é executada
    bool inserido = false;

    for (int j = 0; j < COLUNAS && !inserido; ++j) {
        for (int i = 0; i < LINHAS; ++i) {
            if (!memoria[i][j].second) {  // Verifica se a célula está livre
                memoria[i][j] = {valor, true};  // Insere o valor
                std::cout << "Valor " << valor << " inserido em [" << i << "][" << j << "]" << std::endl;
                inserido = true;
                return;
            }
        }
    }

    if (!inserido) {
        std::cerr << "Erro: Disco cheio!" << std::endl;
    }
}

void Disco::display() const {
    for (int i = 0; i < LINHAS; ++i) {
        for (int j = 0; j < COLUNAS; ++j) {
            int valor = memoria[i][j].first;
            bool preenchido = memoria[i][j].second;

            std::cout << (preenchido ? valor : 0) << " ";
        }
        std::cout << std::endl;
    }
}
