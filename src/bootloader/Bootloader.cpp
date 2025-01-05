#include "../includes/Bootloader.hpp"

// Variáveis para armazenar as configurações
string Bootloader::OUTPUT_LOGS_DIR = "";
int Bootloader::NUM_NUCLEOS = 0;
int Bootloader::QUANTUM_PROCESS_MIN = 0;
int Bootloader::QUANTUM_PROCESS_MAX = 0;

void Bootloader::loadConfigBootloader(const string& file) {
    ifstream inputFile(file);
    if (!inputFile.is_open()) {
        cerr << "Erro ao abrir o arquivo de configuração: " << file << endl;
        exit(EXIT_FAILURE);
    }

    unordered_map<string, string> configs;
    string linha;

    while (getline(inputFile, linha)) {
        size_t igual = linha.find('=');
        if (igual != string::npos) {
            string chave = linha.substr(0, igual);
            string valor = linha.substr(igual + 1);
            configs[chave] = valor;
        }
    }

    inputFile.close();

    if (configs.find("NUM_NUCLEOS") == configs.end() || configs.find("OUTPUT_LOGS_DIR") == configs.end() ||
        configs.find("QUANTUM_PROCESS_MIN") == configs.end() || configs.find("QUANTUM_PROCESS_MAX") == configs.end()) {
        cerr << "Arquivo de configuração ('data/configBootloader.txt') inválido: chaves necessárias estão faltando ou definidas incorretamente." << endl;
        exit(EXIT_FAILURE);
    }

    NUM_NUCLEOS = stoi(configs["NUM_NUCLEOS"]);
    OUTPUT_LOGS_DIR = configs["OUTPUT_LOGS_DIR"];
    QUANTUM_PROCESS_MIN = stoi(configs["QUANTUM_PROCESS_MIN"]);
    QUANTUM_PROCESS_MAX = stoi(configs["QUANTUM_PROCESS_MAX"]);
}

void Bootloader::garantirDiretorioSaidaExiste(const string& path) {
    if (!fs::exists(path)) {
        try {
            fs::create_directories(path);
            cout << "Diretório criado: " << path << endl;
        } catch (const fs::filesystem_error& e) {
            cerr << "Erro ao criar diretório: " << e.what() << endl;
            exit(EXIT_FAILURE);
        }
    }
}

vector<PCB*> Bootloader::createAndConfigPCBs(
    Disco& disco, RAM& ram, Registers& regs, Escalonador& escalonador,
    const vector<string>& arquivosInstrucoes, ofstream& globalLog, int tamanhoRAM) {
    
    // Definindo o tamanho do bloco como constante
    const int TAMANHO_BLOCO = 5; // Cada processo recebe 5 endereços de memória

    // Criando PCBs
    vector<PCB*> pcbs = ProcessManager::createPCBs(disco, ram, regs, arquivosInstrucoes);

    // Alocação de memória para cada processo e adicionando ao escalonador
    for (auto& pcb : pcbs) {
        // Calculando endereço base e limite
        int enderecoBase = (pcb->pid - 1) * TAMANHO_BLOCO;
        int limite = enderecoBase + TAMANHO_BLOCO - 1;

        // Verificando se o limite ultrapassa o tamanho da RAM
        if (limite >= tamanhoRAM) {
            cerr << "Erro: Memória insuficiente para alocar o processo " << pcb->pid << endl;
            globalLog << "Erro: Memória insuficiente para alocar o processo " << pcb->pid << endl;
            exit(EXIT_FAILURE);
        }

        // Alocando memória
        pcb->alocarMemoria(ram, enderecoBase, limite);

        // Adicionando o processo ao escalonador
        escalonador.adicionarProcesso(pcb, globalLog);
    }

    return pcbs;
}


void Bootloader::createCores(vector<Core>& cores, int numNucleos, RAM& ram, Disco& disco, Escalonador& escalonador) {
    // Criando múltiplos núcleos
    for (int i = 0; i < numNucleos; ++i) {
        cores.push_back(Core(ram, disco, escalonador));
    }
}

void Bootloader::inicializarSistema(vector<Core>& cores, Disco& disco, Escalonador& escalonador, Perifericos& periferico, RAM& ram, Registers& regs){
    // Garantindo que o diretório de saída exista ("data/logs")
    garantirDiretorioSaidaExiste(OUTPUT_LOGS_DIR);

    // Criando arquivo de log global ("data/global_log.txt")
    ofstream globalLog(string(OUTPUT_LOGS_DIR) + "/global_log.txt", ios::out);
    if (!globalLog.is_open()) {
        cerr << "Erro ao abrir o arquivo de log global.\n";
        return;
    }

    // Configurando os registradores
    disco.setRegistersFromFile(regs, "data/setRegisters.txt");

    // Lista de arquivos de instrução
    vector<string> arquivosInstrucoes = disco.listInstructionsFile("data/instr");
    
    // Definindo o tamanho da RAM (exemplo: 64 endereços)
    int tamanhoRAM = ram.getSize();

    // Criando e configurando PCBs
    vector<PCB*> pcbs = Bootloader::createAndConfigPCBs(disco, ram, regs, escalonador, arquivosInstrucoes, globalLog, tamanhoRAM);

    // **Teste 1: Verificar PCBs Criados**
    globalLog << "\n=== Teste: PCBs Criados ===\n";
    for (auto& pcb : pcbs) {
        pcb->exibirPCB(globalLog); // Exibe informações dos PCBs criados
    }

    // Criando múltiplos núcleos
    createCores(cores, NUM_NUCLEOS, ram, disco, escalonador);

    // **Teste 2: Verificar RAM Após Alocação**
    globalLog << "\n=== Teste: Estado da RAM Após Alocação ===\n";
    ram.display(globalLog); // Exibe o estado da RAM após a alocação de memória


    // Executando os núcleos em threads
    vector<thread> threads;
    for (auto& core : cores) {
        threads.push_back(thread(&Core::run, &core)); // Rodando cada núcleo em uma thread
    }

    // Aguardando que todas as threads terminem
    for (auto& th : threads) {
        th.join(); // Espera todas as threads terminarem
    }

    // **Teste 3: Verificar Estado Final da RAM**
    globalLog << "\n=== Teste: Estado Final da RAM ===\n";
    ram.display(globalLog);

    // Exibindo o estado final do Disco
    globalLog << "\n===== Estado Final do Disco =====\n";
    disco.display(globalLog);

    // Liberando memória dos processos
    for (auto& pcb : pcbs) {
        pcb->liberarMemoria(ram);
        delete pcb;
    }

    globalLog.close();
}
