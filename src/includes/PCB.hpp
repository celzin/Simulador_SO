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
};

#endif
