#include "../includes/RAM.hpp"

// Construtor com validação do tamanho
RAM::RAM(int tamanho) : size(tamanho), memoria(tamanho, 0), instruction_memory(tamanho) {
    if (tamanho % TAMANHO_QUADRO != 0) {
        throw std::invalid_argument("Erro: Tamanho da RAM deve ser múltiplo de TAMANHO_QUADRO.");
    }
    totalQuadros = tamanho / TAMANHO_QUADRO;
    quadrosUsados.resize(totalQuadros, false);
    std::cout << "[DEBUG] RAM criada com tamanho: " << tamanho 
              << " bytes, Total de Quadros: " << totalQuadros << std::endl;
}

// Escrita na memória por endereço
void RAM::write(int endereco, int valor) {
    std::lock_guard<std::mutex> lock(mtx);
    if (endereco >= 0 && endereco < memoria.size()) {
        memoria[endereco] = valor;
    } else {
        std::cout << "Erro: Endereço fora da memória!" << std::endl;
    }
}

// Leitura na memória por endereço
int RAM::read(int endereco) {
    std::lock_guard<std::mutex> lock(mtx);
    if (endereco >= 0 && endereco < memoria.size()) {
        return memoria[endereco];
    } else {
        std::cout << "Erro: Endereço fora da memória!" << std::endl;
        return -1;
    }
}

// Escrita de instrução na memória
void RAM::writeInstruction(int endereco, const Instruction& instr) {
    std::lock_guard<std::mutex> lock(mtx);
    if (endereco >= 0 && endereco < instruction_memory.size()) {
        instruction_memory[endereco] = instr;
    } else {
        std::cout << "Erro: Endereço inválido para instrução na RAM " << endereco << std::endl;
    }
}

// Busca de instrução na memória
Instruction RAM::fetchInstruction(int endereco) const {
    if (endereco >= 0 && endereco < instruction_memory.size()) {
        return instruction_memory[endereco];
    }
    std::cout << "Erro: Endereço inválido para instrução na RAM " << endereco << std::endl;
    return Instruction(ADD, 0, 0, 0);
}

// Verifica se o endereço está reservado
bool RAM::isReserved(int endereco) const {
    if (endereco < 0 || endereco >= memoria.size()) {
        return true; // Fora do limite, considerado reservado
    }
    return memoria[endereco] != 0; // Qualquer valor diferente de 0 indica que está reservado
}

// Exibição do conteúdo da memória
void RAM::display(std::ofstream& outfile) const {
    for (int i = 0; i < totalQuadros; i++) {
        outfile << "Quadro " << i << " -> ";
        int inicio = i * TAMANHO_QUADRO;
        int fim = inicio + TAMANHO_QUADRO;
        for (int j = inicio; j < fim; ++j) {
            outfile << memoria[j] << " ";
        }
        outfile << std::endl;
    }
}

// Exibição das instruções na memória
void RAM::displayI() const {
    for (int i = 0; i < instruction_memory.size(); i++) {
        std::cout << "Endereço " << i << " -> " << instruction_memory[i] << std::endl;
    }
}

// Gerenciamento de Quadros: Alocar
int RAM::alocarQuadro() {
    std::lock_guard<std::mutex> lock(mtx);
    for (int i = 0; i < totalQuadros; ++i) {
        if (!quadrosUsados[i]) {
            quadrosUsados[i] = true;
            std::cout << "[DEBUG] Quadro " << i << " alocado." << std::endl;
            return i;
        }
    }
    throw std::runtime_error("Erro: Memória cheia, nenhum quadro disponível.");
}

// Gerenciamento de Quadros: Liberar
void RAM::liberarQuadro(int indiceQuadro) {
    std::lock_guard<std::mutex> lock(mtx);
    if (indiceQuadro < 0 || indiceQuadro >= totalQuadros) {
        throw std::invalid_argument("Erro: Índice de quadro inválido.");
    }
    quadrosUsados[indiceQuadro] = false;
    std::cout << "[DEBUG] Quadro " << indiceQuadro << " liberado." << std::endl;
}

// Escrita em um quadro específico
void RAM::escreverNoQuadro(int indiceQuadro, const std::vector<int>& dados) {
    std::lock_guard<std::mutex> lock(mtx);
    if (indiceQuadro < 0 || indiceQuadro >= totalQuadros) {
        throw std::invalid_argument("Erro: Índice de quadro inválido.");
    }
    int inicio = indiceQuadro * TAMANHO_QUADRO;
    for (size_t i = 0; i < dados.size() && inicio + i < memoria.size(); ++i) {
        memoria[inicio + i] = dados[i];
    }
}

// Leitura de um quadro específico
std::vector<int> RAM::lerQuadro(int indiceQuadro) {
    std::lock_guard<std::mutex> lock(mtx);
    if (indiceQuadro < 0 || indiceQuadro >= totalQuadros) {
        throw std::invalid_argument("Erro: Índice de quadro inválido.");
    }
    int inicio = indiceQuadro * TAMANHO_QUADRO;
    return std::vector<int>(memoria.begin() + inicio, memoria.begin() + inicio + TAMANHO_QUADRO);
}
