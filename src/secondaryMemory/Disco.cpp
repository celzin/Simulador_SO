#include "../includes/Disco.hpp"

Disco::Disco() : memoria(LINHAS, std::vector<std::pair<int, bool>>(COLUNAS, {0, false})) {}

void Disco::write(int valor) {
    bool inserido = false;

    for (int j = 0; j < COLUNAS && !inserido; ++j) {
        for (int i = 0; i < LINHAS; ++i) {
            if (!memoria[i][j].second) { 
                memoria[i][j] = {valor, true}; 
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

void Disco::setRegistersFromFile(Registers& regs, const string& regsFilename) {
    ifstream regsFile(regsFilename);
    if (!regsFile.is_open()) {
        cerr << "Erro ao abrir o arquivo de registradores: " << regsFilename << endl;
        return;
    }

    string line;
    while (getline(regsFile, line)) {
        int regNum, regValue;
        char virgula;
        stringstream ss(line);

        ss >> regNum >> virgula >> regValue;
        regs.set(regNum, regValue); 
    }
    regsFile.close();
}

int Disco::loadInstructionsFromFile(RAM& ram, const std::string& instrFilename, int baseAddress) {
    std::ifstream file(instrFilename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de instruções: " << instrFilename << std::endl;
        return -1;
    }

    std::string line;
    int instructionAddress = baseAddress; // Endereço base na RAM
    int quantumProcesso = 0;              // Quantum do processo
    int count = 0;                        // Número de instruções

    // Ler quantum do processo na primeira linha
    if (std::getline(file, line)) {
        try {
            quantumProcesso = std::stoi(line); // Converter para inteiro
        } catch (...) {
            std::cerr << "Erro: Quantum inválido no arquivo " << instrFilename << std::endl;
            return -1;
        }
    }

    // Ler as instruções restantes
    while (std::getline(file, line)) {
        std::string opcodeStr;
        int reg1, reg2, reg3;
        char virgula;

        std::stringstream ss(line);
        std::getline(ss, opcodeStr, ',');
        ss >> reg1 >> virgula >> reg2 >> virgula >> reg3;

        Opcode opcode;
        if (opcodeStr == "ADD") opcode = ADD;
        else if (opcodeStr == "SUB") opcode = SUB;
        else if (opcodeStr == "AND") opcode = AND;
        else if (opcodeStr == "OR") opcode = OR;
        else if (opcodeStr == "STORE") opcode = STORE;
        else if (opcodeStr == "LOAD") opcode = LOAD;
        else if (opcodeStr == "ENQ") opcode = ENQ;
        else if (opcodeStr == "IF_igual") opcode = IF_igual;
        else {
            std::cerr << "Erro: Instrução inválida no arquivo " << opcodeStr << std::endl;
            continue;
        }

        Instruction instr(opcode, reg1, reg2, reg3);
        if (instructionAddress < ram.tamanho) {
            ram.writeInstruction(instructionAddress++, instr);
            count++;
        } else {
            std::cerr << "Erro: Memória RAM cheia, não é possível carregar mais instruções." << std::endl;
            break;
        }
    }
    file.close();

    return quantumProcesso; // Retorna o quantum do processo
}


vector<string> Disco::listInstructionsFiles(const string& listDir, const string& extension){
    vector<string> files;
    try {
        for(const auto& entry : filesystem::directory_iterator(listDir)){
            if(entry.is_regular_file() && entry.path().extension() == extension){
                files.push_back(entry.path().string());
            }
        }
    } catch (const filesystem::filesystem_error& e) {
        cerr << "Erro ao acessar lista de arquivos no diretório: " << listDir << endl;
    }
    return files;
}

vector<pair<string,int>> Disco::loadProcessFromFiles(RAM& ram, const string& diretorio){
    vector<pair<string, int>> processos;
    vector<string> files = listInstructionsFiles(diretorio, ".txt");

    int baseAddress = 0;

    for(const auto& file : files){
        int instructionCount = loadInstructionsFromFile(ram, file, baseAddress);
        if(instructionCount > 0){
            processos.emplace_back(file, instructionCount);
            baseAddress += instructionCount;
        } else {
            cerr << "Erro ao carregar instruções do arquivo: " << file << endl;
        }
    }
    return processos;
}