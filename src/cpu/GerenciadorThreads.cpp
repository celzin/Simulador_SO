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

                std::cout << "\n[Core " << core->id << "] Iniciando processo " << processo->pid << ".\n";
                core->setProcesso(processo);
                core->executarProcesso();
                std::cout << "[Core " << core->id << "] Processo " << processo->pid << " finalizado.\n";
            }

            std::cout << "[Core " << core->id << "] Nenhum processo restante. Finalizando thread.\n";
        });
    }

    // Aguardar a finalização de todas as threads
    for (std::thread& t : threads) {
        t.join();
    }
}

