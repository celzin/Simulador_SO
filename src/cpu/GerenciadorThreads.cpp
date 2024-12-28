#include "../includes/GerenciadorThreads.hpp"

GerenciadorThreads::GerenciadorThreads(std::vector<Core*>& cores, GerenciadorProcessos& gp)
    : cores(cores), gerenciadorProcessos(gp) {}

void GerenciadorThreads::iniciar() {
    std::vector<std::thread> threads;

    for (Core* core : cores) {
        threads.emplace_back([core, this]() {
            while (true) {
                PCB* processo = nullptr;

                // Verificar se há processos disponíveis
                {
                    std::lock_guard<std::mutex> lock(this->mutex);
                    if (gerenciadorProcessos.temProcessos()) {
                        processo = gerenciadorProcessos.obterProximoProcesso();
                    }
                }

                if (processo == nullptr) break; // Sem mais processos para executar

                std::cout << "\n\033[34m[Core " << core->id << "] Iniciando processo " << processo->pid << ".\033[0m\n";
                core->setProcesso(processo);
                core->executarProcesso();
                std::cout << "\033[32m[Core " << core->id << "] Processo " << processo->pid << " finalizado.\033[0m\n";
            }

            std::cout << "\033[31m[Core " << core->id << "] Nenhum processo restante. Finalizando thread.\033[0m\n";
        });
    }

    // Aguardar a finalização de todas as threads
    for (std::thread& t : threads) {
        t.join();
    }
}

