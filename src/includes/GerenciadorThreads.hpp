#ifndef GERENCIADOR_THREADS_HPP
#define GERENCIADOR_THREADS_HPP

#include <vector>
#include <thread>
#include <mutex>
#include "Core.hpp"
#include "GerenciadorProcessos.hpp"

class GerenciadorThreads {
private:
    std::vector<Core*> cores; // Núcleos disponíveis
    GerenciadorProcessos& gerenciadorProcessos;
    std::mutex mutex; // Para sincronização de recursos

public:
    GerenciadorThreads(std::vector<Core*>& cores, GerenciadorProcessos& gp);
    void iniciar(); // Inicia a execução dos processos nos núcleos
};

#endif
