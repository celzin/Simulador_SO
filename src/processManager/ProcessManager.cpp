#include "../includes/ProcessManager.hpp"

vector<PCB*> ProcessManager::createPCBs(Disco& disco, RAM& ram, Registers& regs, const vector<string>& arquivosInstrucoes) {
    vector<PCB*> pcbs;
    int enderecoAtual = 0; // Endereço inicial na RAM

    // Configurando gerador aleatório de números para o quantum
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(Bootloader::QUANTUM_PROCESS_MIN, Bootloader::QUANTUM_PROCESS_MAX);

    // Criando pcbs a partir da lista de arquivos de instruções e carregando as instruções na RAM
    for (int i = 0; i < arquivosInstrucoes.size(); ++i) {
        vector<int> quantidadeInstrucoes = disco.loadInstructionsFromFile(arquivosInstrucoes[i]);
        if (quantidadeInstrucoes.empty()) {
            cerr << "Erro ao quantificar instruções do arquivo: " << arquivosInstrucoes[i] << endl;
            continue;
        }

        // Gera um quantum aleatório para o PCB
        int quantumRandom = distrib(gen);

        // Cria o PCB associado à faixa de memória de instruções
        PCB* novoPCB = new PCB(i + 1, quantumRandom, regs, 0, quantidadeInstrucoes.size() - 1);

        // Dividir instruções em páginas virtuais e mapear para quadros físicos
        for (size_t pagina = 0; pagina * RAM::TAMANHO_QUADRO < quantidadeInstrucoes.size(); ++pagina) {
            size_t inicio = pagina * RAM::TAMANHO_QUADRO;
            size_t fim = min(inicio + RAM::TAMANHO_QUADRO, quantidadeInstrucoes.size());

            // Extraia os dados da página
            vector<int> dadosPagina(quantidadeInstrucoes.begin() + inicio, quantidadeInstrucoes.begin() + fim);

            // Alocar quadro físico na RAM
            int quadro = ram.alocarQuadro();
            ram.escreverNoQuadro(quadro, dadosPagina);

            // Mapear a página virtual para o quadro físico
            novoPCB->mapearPaginaParaQuadro(pagina, quadro);
        }

        novoPCB->PC = 0;
        pcbs.push_back(novoPCB);
    }

    cout << endl;

    return pcbs;
}
