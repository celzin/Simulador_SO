#ifndef PCB_HPP
#define PCB_HPP

#include <vector>
#include <string>
#include <iostream>
#include <algorithm> 

enum ProcessState { Ready, Blocked, Running };

class ProcessControlBlock {
public:
    int process_id;
    ProcessState state;
    int pc;
    int quantum;
    std::vector<int> registradores;
    int memory_page_id;
    std::vector<std::string> resources;
    int timestamp;

    // Construtor
    ProcessControlBlock(int id, int page_id, int quantum);

    // Métodos auxiliares
    void setState(ProcessState new_state);
    ProcessState getState() const;

    void updatePC(int new_pc);
    int getPC() const;

    void setQuantum(int new_quantum);
    int getQuantum() const;

    void addResource(const std::string& resource);
    void removeResource(const std::string& resource);

    void display() const {
        std::cout << std::endl << "Process ID: " << process_id << "\n"
                  << "State: " << (state == Ready ? "Pronto" : state == Blocked ? "Bloqueado" : "Executando") << "\n"
                  << "PC: " << pc << "\n"
                  << "Quantum: " << quantum << "\n"
                  << "Memory Page ID: " << memory_page_id << "\n"
                  << "Resources: ";
        for (const auto& res : resources) {
            std::cout << res << " ";
        }
        std::cout << "\n-----------------------------\n";
    }
};

#endif
