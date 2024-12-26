#include "includes/ProcessManager.hpp"
#include "includes/Core.hpp"
#include <thread>
#include <vector>
#include <memory>

#define NUM_CORES 2

void gerenciarRecursos(ProcessManager& pm) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        pm.desbloquearProcessos("teclado");
        pm.desbloquearProcessos("disco");
    }
}

int main() {
    RAM ram;
    Disco disco;
    ProcessManager processManager;

    processManager.setProcessFromFile("data/setProcess.txt");

    std::vector<std::unique_ptr<Core>> cores;

    for (int i = 0; i < NUM_CORES; ++i) {
        cores.emplace_back(std::make_unique<Core>(ram, disco, processManager));
        cores[i]->start();
    }

    std::thread gerenciador(gerenciarRecursos, std::ref(processManager));

    std::this_thread::sleep_for(std::chrono::seconds(10));

    for (auto& core : cores) {
        core->stop();
    }

    gerenciador.detach();

    std::cout << "Execução concluída. Todos os núcleos foram parados.\n";

    return 0;
}
