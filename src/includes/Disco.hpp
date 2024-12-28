#ifndef DISCO_HPP
#define DISCO_HPP

#include "Registers.hpp"
#include "RAM.hpp"
#include <iostream>
#include <vector>

class Disco {
private:
    static const int LINHAS = 10;
    static const int COLUNAS = 10;
    
    std::vector<std::vector<std::pair<int, bool>>> memoria;

public:
    Disco(); 

    void write(int valor); 
    void display() const;
    void setRegistersFromFile(Registers& regs, const std::string& regsFilename);
    int loadInstructionsFromFile(RAM& ram, const std::string& instrFilename);
};

#endif