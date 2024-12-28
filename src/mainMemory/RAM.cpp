#include "../includes/RAM.hpp"

RAM::RAM() : memoria(tamanho, 0), instruction_memory(tamanho), paginasOcupadas(tamanho / tamanhoPagina, false) {}

// Aloca uma página para um processo
int RAM::alocarPagina(int processoId) {
    for (size_t i = 0; i < paginasOcupadas.size(); ++i) {
        if (!paginasOcupadas[i]) {
            paginasOcupadas[i] = true;
            tabelaPaginas[{processoId, i}] = i * tamanhoPagina; // Mapeia para o processo e página lógica
            std::cout << "[RAM] Página lógica " << i << " do processo " << processoId 
                      << " mapeada para endereço físico " << i * tamanhoPagina << std::endl;
            return i;
        }
    }
    std::cerr << "[RAM] Erro: Não há páginas disponíveis na RAM!" << std::endl;
    return -1;
}

// Libera páginas associadas a um processo
void RAM::liberarPagina(int processoId) {
    bool liberada = false;

    for (auto it = tabelaPaginas.begin(); it != tabelaPaginas.end();) {
        // Verifica se a entrada corresponde ao processoId
        if (it->first.first == processoId) {
            int enderecoFisico = it->second; // Obtém o endereço físico
            int paginaFisica = enderecoFisico / tamanhoPagina; // Calcula a página física a partir do endereço
            paginasOcupadas[paginaFisica] = false; // Marca a página física como livre

            std::cout << "[RAM] Página lógica " << it->first.second 
                      << " liberada do processo " << processoId << std::endl;

            it = tabelaPaginas.erase(it); // Remove a entrada da tabela de páginas
            liberada = true;
        } else {
            ++it;
        }
    }

    if (!liberada) {
        std::cerr << "[RAM] Erro: Nenhuma página alocada para o processo " 
                  << processoId << " foi encontrada!" << std::endl;
    }
}

void RAM::write(int endereco, int valor) {
    int paginaLogica = endereco / tamanhoPagina; // Página lógica
    int offset = endereco % tamanhoPagina;       // Offset

    auto it = tabelaPaginas.find({currentProcessId, paginaLogica});
    if (it != tabelaPaginas.end()) {
        int enderecoFisico = it->second + offset;
        if (enderecoFisico >= 0 && enderecoFisico < tamanho) {
            memoria[enderecoFisico] = valor;
            std::cout << "[RAM] Valor " << valor << " escrito no endereço físico " << enderecoFisico 
                      << " (Página Lógica: " << paginaLogica << ", Offset: " << offset << ")" << std::endl;
            return;
        } else {
            std::cerr << "[RAM] Erro: Endereço físico " << enderecoFisico << " fora da RAM!" << std::endl;
        }
    } else {
        std::cerr << "[RAM] Erro: Página lógica " << paginaLogica 
                  << " do processo " << currentProcessId << " não mapeada para RAM!" << std::endl;
    }
}

int RAM::read(int endereco) {
    int paginaLogica = endereco / tamanhoPagina; // Página lógica
    int offset = endereco % tamanhoPagina;       // Offset

    auto it = tabelaPaginas.find({currentProcessId, paginaLogica});
    if (it != tabelaPaginas.end()) {
        int enderecoFisico = it->second + offset;
        if (enderecoFisico >= 0 && enderecoFisico < tamanho) {
            std::cout << "[RAM] Valor " << memoria[enderecoFisico] << " lido do endereço físico " << enderecoFisico 
                      << " (Página Lógica: " << paginaLogica << ", Offset: " << offset << ")" << std::endl;
            return memoria[enderecoFisico];
        } else {
            std::cerr << "[RAM] Erro: Endereço físico " << enderecoFisico << " fora da RAM!" << std::endl;
        }
    } else {
        std::cerr << "[RAM] Erro: Página lógica " << paginaLogica 
                  << " do processo " << currentProcessId << " não mapeada para RAM!" << std::endl;
    }
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
