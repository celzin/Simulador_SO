#include "../includes/ProcessManager.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

ProcessManager::ProcessManager() {
    // Inicializa todos os recursos como disponíveis
    recursos = {{"teclado", true}, {"disco", true}};
}

void ProcessManager::adicionarProcesso(const ProcessControlBlock& pcb) {
    std::lock_guard<std::mutex> lock(mtx);
    filaProntos.push(pcb);
}

ProcessControlBlock ProcessManager::obterProximoProcesso() {
    std::lock_guard<std::mutex> lock(mtx);
    if (!filaProntos.empty()) {
        ProcessControlBlock pcb = filaProntos.front();
        filaProntos.pop();
        return pcb;
    }
    return ProcessControlBlock(-1, 0, 0); // Nenhum processo disponível
}

void ProcessManager::bloquearProcesso(ProcessControlBlock& pcb, const std::string& recurso) {
    std::lock_guard<std::mutex> lock(mtx);

    // Marca o recurso como ocupado
    recursos[recurso] = false;

    // Altera o estado do processo e move para a fila de bloqueados
    pcb.state = BLOQUEADO;
    pcb.resource = recurso;
    filaBloqueados.push(pcb);

    std::cout << "Processo " << pcb.processID << " bloqueado aguardando recurso: " << recurso << std::endl;
}

void ProcessManager::desbloquearProcessos(const std::string& recurso) {
    std::lock_guard<std::mutex> lock(mtx);

    std::queue<ProcessControlBlock> tempQueue;
    while (!filaBloqueados.empty()) {
        ProcessControlBlock pcb = filaBloqueados.front();
        filaBloqueados.pop();

        if (pcb.resource == recurso) {
            pcb.state = PRONTO;
            filaProntos.push(pcb);
            std::cout << "Processo " << pcb.processID << " desbloqueado. Recurso: " << recurso << " liberado." << std::endl;
        } else {
            tempQueue.push(pcb);
        }
    }
    filaBloqueados = tempQueue;

    // Marca o recurso como disponível
    recursos[recurso] = true;
}

bool ProcessManager::temProcessosProntos() {
    std::lock_guard<std::mutex> lock(mtx);
    return !filaProntos.empty();
}

void ProcessManager::setProcessFromFile(const std::string& processFilename){
    std::ifstream processFile(processFilename);

    if(!processFile.is_open()){
        std::cerr << "Erro ao abrir o arquivo de processos: " << processFilename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(processFile, line)){
        int processID, processPC, processQuantum;
        char comma1, comma2;
        std::stringstream ss(line);

        if(!(ss >> processID >> comma1 >> processPC >> comma2 >> processQuantum) || comma1 != ',' || comma2 != ','){
            std::cerr << "Linha inválida no arquivo de processos: " << line << std::endl;
            continue;
        }

        adicionarProcesso(ProcessControlBlock(processID, processPC, processQuantum));
    }
}
