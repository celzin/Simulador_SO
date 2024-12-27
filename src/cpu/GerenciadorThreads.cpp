#include "../includes/GerenciadorThreads.hpp"

GerenciadorThreads::GerenciadorThreads(std::vector<Core*>& cores, GerenciadorProcessos& gp)
    : cores(cores), gerenciadorProcessos(gp) {}

void GerenciadorThreads::iniciar() {
    std::vector<std::thread> threads;

    for (Core* core : cores) {
        threads.emplace_back([core, this]() {
            while (true) {
                PCB* processo;

                // Acessar a fila de prontos com exclusão mútua
                {
                    std::lock_guard<std::mutex> lock(this->mutex);
                    processo = gerenciadorProcessos.obterProximoProcesso();
                }

                if (processo == nullptr) break; // Sem mais processos para executar

                core->setProcesso(processo);
                core->executarProcesso();
            }
        });
    }

    // Aguardar a finalização de todas as threads
    for (std::thread& t : threads) {
        t.join();
    }
}
