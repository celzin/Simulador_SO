#ifndef PCB_HPP
#define PCB_HPP

#include "Registers.hpp"
#include "Perifericos.hpp"
#include "RAM.hpp"

#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

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
    int enderecoBaseInstrucoes;
    int enderecoLimiteInstrucoes;
    EstadoProcesso estado;
    Registers registradores;
    Perifericos recursos; 

    vector<int> memoriaAlocada;  // Memória alocada ao processo (endereço base e limite)
    vector<int> estadoPipeline;  // Estado do pipeline (valores intermediários)
    vector<int> tabelaPaginas;   // Mapeia páginas virtuais para quadros físicos


    PCB(int id, int quantum, const Registers& regs, int enderecoBaseInstrucoes, int enderecoLimiteInstrucoes);

    void mapearPaginaParaQuadro(int pagina, int quadro);
    int traduzirEnderecoVirtual(int enderecoVirtual);

    void salvarEstado(const vector<int>& pipelineState);
    void restaurarEstado(vector<int>& pipelineState, ofstream& outfile);
    
    void decrementarQuantum(ofstream& outfile);
    bool quantumExpirado() const;
    void resetarQuantum(ofstream& outfile);

    void atualizarEstado(EstadoProcesso novoEstado, ofstream& outfile); 
    bool verificarEstado(EstadoProcesso verEstado) const; 

    void alocarMemoria(RAM& ram, int enderecoBase, int limite); 
    bool verificarAcessoMemoria(int endereco) const;
    void liberarMemoria(RAM& ram);

    void associarRecurso(const string& nomeRecurso, bool alocado);
    bool verificarRecurso(const string& nomeRecurso) const;

    void exibirPCB(ofstream& outfile) const;

    int getEnderecoBaseInstrucoes() const;
    int getLimiteInstrucoes() const;
};

#endif
