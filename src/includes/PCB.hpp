#ifndef PCB_HPP
#define PCB_HPP

#include "Registers.hpp"
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>

enum EstadoProcesso {
    PRONTO,
    EXECUCAO,
    BLOQUEADO,
    FINALIZADO
};

class PCB {
public:
    int pid;
    EstadoProcesso estado;
    int PC;
    Registers registradores;
    int quantumProcesso;
    int quantumRestante;

    std::vector<int> memoriaAlocada;  // Memória alocada ao processo (endereço base e limite)
    std::vector<int> estadoPipeline;  // Estado do pipeline (valores intermediários)
    std::unordered_map<std::string, bool> recursos; // Recursos associados ao processo

    PCB(int id, int quantum);

    // Métodos para salvar e restaurar estado
    void salvarEstado();
    void restaurarEstado();
    void decrementarQuantum();
    bool quantumExpirado() const;

    // Gerenciamento do pipeline
    void salvarEstadoPipeline(const std::vector<int>& pipelineState);
    std::vector<int> obterEstadoPipeline() const;

    // Gerenciamento de memória
    void alocarMemoria(int enderecoBase, int limite);
    void liberarMemoria();

    // Gerenciamento de recursos
    void associarRecurso(const std::string& nomeRecurso, bool alocado);
    bool verificarRecurso(const std::string& nomeRecurso) const;

    // Exibição das informações do PCB
    void exibirPCB() const;
};

#endif
