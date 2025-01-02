#include "../includes/ProcessManager.hpp"

#define QUANTUM_MIN 20
#define QUANTUM_MAX 50

vector<PCB*> ProcessManager::createPCBs(Disco& disco, RAM& ram, Registers& regs, const vector<string>& arquivosInstrucoes) {
    vector<PCB*> pcbs;
    int enderecoAtual = 0; // Endereço inicial na RAM

    // Configurando gerador aleatório de números para o quantum
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(QUANTUM_MIN, QUANTUM_MAX);

    // Criando pcbs a partir da lista de arquivos de instruções e carregando as instruções na RAM
    for (int i = 0; i < arquivosInstrucoes.size(); ++i) {
        int quantidadeInstrucoes = disco.loadInstructionsFromFile(ram, arquivosInstrucoes[i], enderecoAtual);
        if (quantidadeInstrucoes == -1) {
            cerr << "Erro ao quantificar instruções do arquivo: " << arquivosInstrucoes[i] << endl;
            continue;
        }

        // Gera um quantum aleatório para o PCB
        int quantumRandom = distrib(gen);

        // Cria o PCB associado à faixa de memória de instruções
        PCB* novoPCB = new PCB(i + 1, 50, regs, enderecoAtual, enderecoAtual + quantidadeInstrucoes - 1);

        // Configura o PC inicial do processo
        novoPCB->PC = enderecoAtual;

        pcbs.push_back(novoPCB);

        cout << "Processo " << i + 1
                  << ": Base Instruções = " << enderecoAtual
                  << ", Limite Instruções = " << enderecoAtual + quantidadeInstrucoes - 1
                  << ", PC Inicial = " << novoPCB->PC 
                  << ", Quantum = " << quantumRandom
                  << endl;

        // Atualiza o endereço base para o próximo conjunto de instruções
        enderecoAtual += quantidadeInstrucoes;
    }

    cout << endl;

    return pcbs;
}
