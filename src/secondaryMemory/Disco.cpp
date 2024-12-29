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

int Disco::loadInstructionsFromFile(RAM& ram, const string& instrFilename, int baseAddress) {
    ifstream file(instrFilename);
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo de instruções: " << instrFilename << endl;
        return -1;
    }

    string line;
    int instructionAddress = baseAddress; // Inicia no endereço base fornecido
    int count = 0; // Conta o número de instruções lidas

    while (getline(file, line)) {
        string opcodeStr;
        int reg1, reg2, reg3;
        char virgula;

        stringstream ss(line);
        getline(ss, opcodeStr, ',');

        opcodeStr.erase(remove_if(opcodeStr.begin(), opcodeStr.end(), ::isspace), opcodeStr.end());
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
            cerr << "Erro: Instrução inválida no arquivo: " << opcodeStr << endl;
            continue;
        }

        // Cria a instrução e grava na RAM
        Instruction instr(opcode, reg1, reg2, reg3);
        if (instructionAddress < ram.tamanho) {
            ram.writeInstruction(instructionAddress++, instr);
            count++;
        } else {
            cerr << "Erro: memória RAM cheia, não é possível carregar mais instruções." << endl;
            break;
        }
    }
    file.close();

    return count; // Retorna o número de instruções lidas
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