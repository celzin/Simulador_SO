#ifndef PCB_HPP
#define PCB_HPP

#include "Registers.hpp"
#include "Perifericos.hpp"
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
    int PC;
    int quantumProcesso;
    int quantumRestante;
    EstadoProcesso estado;
    Registers registradores;
    Perifericos recursos; 

    std::vector<int> memoriaAlocada;  // Memória alocada ao processo (endereço base e limite)
    std::vector<int> estadoPipeline;  // Estado do pipeline (valores intermediários)
    // std::unordered_map<std::string, bool> recursos; // Recursos associados ao processo

    PCB(int id, int quantum, const Registers& regs);

    void salvarEstado(const std::vector<int>& pipelineState);
    void restaurarEstado(std::vector<int>& pipelineState);
    
    void decrementarQuantum();
    bool quantumExpirado() const;

    void atualizarEstado(EstadoProcesso novoEstado); 
    bool verificarEstado(EstadoProcesso verEstado) const; 

    void alocarMemoria(int enderecoBase, int limite);
    bool verificarAcessoMemoria(int endereco) const;
    void liberarMemoria();

    void associarRecurso(const std::string& nomeRecurso, bool alocado);
    bool verificarRecurso(const std::string& nomeRecurso) const;

    void exibirPCB() const;
};

#endif
