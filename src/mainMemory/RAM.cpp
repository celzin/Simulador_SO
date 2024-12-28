#include "../includes/RAM.hpp"

RAM::RAM() : memoria(tamanho, 0), instruction_memory(tamanho), paginasOcupadas(tamanho / tamanhoPagina, false) {}

// Aloca uma página para um processo
int RAM::alocarPagina(int processoId) {
    for (size_t i = 0; i < paginasOcupadas.size(); ++i) {
        if (!paginasOcupadas[i]) {
            paginasOcupadas[i] = true;
            tabelaPaginas[processoId * tamanhoPagina + i] = i * tamanhoPagina; // Mapeia a página para o processo
            return i;
        }
    }
    std::cerr << "Erro: Não há páginas disponíveis na RAM!" << std::endl;
    return -1;
}

// Libera páginas associadas a um processo
void RAM::liberarPagina(int processoId) {
    for (auto it = tabelaPaginas.begin(); it != tabelaPaginas.end();) {
        if (it->first / tamanhoPagina == processoId) {
            paginasOcupadas[it->second / tamanhoPagina] = false;
            it = tabelaPaginas.erase(it);
        } else {
            ++it;
        }
    }
}

// Escrita considerando paginação
void RAM::write(int endereco, int valor) {
    auto it = tabelaPaginas.find(endereco / tamanhoPagina);
    if (it != tabelaPaginas.end()) {
        int paginaFisica = it->second;
        int offset = endereco % tamanhoPagina;
        memoria[paginaFisica + offset] = valor;
    } else {
        std::cerr << "Erro: Endereço lógico não mapeado para RAM!" << std::endl;
    }
}

// Leitura considerando paginação
int RAM::read(int endereco) {
    auto it = tabelaPaginas.find(endereco / tamanhoPagina);
    if (it != tabelaPaginas.end()) {
        int paginaFisica = it->second;
        int offset = endereco % tamanhoPagina;
        return memoria[paginaFisica + offset];
    }
    std::cerr << "Erro: Endereço lógico não mapeado para RAM!" << std::endl;
    return -1;
}

// Escreve uma instrução considerando paginação
void RAM::writeInstruction(int endereco, const Instruction& instr) {
    if (endereco >= 0 && endereco < tamanho) {
        instruction_memory[endereco] = instr;
    } else {
        std::cerr << "Erro: Endereço inválido para instrução na RAM!" << std::endl;
    }
}

// Lê uma instrução considerando paginação
Instruction RAM::fetchInstruction(int endereco) const {
    if (endereco >= 0 && endereco < tamanho) {
        return instruction_memory[endereco];
    }
    std::cerr << "Erro: Endereço inválido para instrução na RAM!" << std::endl;
    return Instruction(ADD, 0, 0, 0);
}

// Exibe o estado da RAM
void RAM::display() const {
    for (size_t i = 0; i < memoria.size(); ++i) {
        std::cout << "Endereço " << i << " -> " << memoria[i] << std::endl;
    }
}

// Exibe as instruções armazenadas
void RAM::displayI() const {
    for (size_t i = 0; i < instruction_memory.size(); ++i) {
        std::cout << "Endereço " << i << " -> " << instruction_memory[i] << std::endl;
    }
}
