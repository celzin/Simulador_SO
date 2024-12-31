#ifndef DISCO_HPP
#define DISCO_HPP

#include "../includes/Registers.hpp"
#include "../includes/RAM.hpp"

#include <iostream>
#include <vector>
#include <mutex> 
#include <fstream>
#include <sstream>
#include <algorithm>

class Disco {
private:
    static const int LINHAS = 10;
    static const int COLUNAS = 10;
    std::vector<std::vector<std::pair<int, bool>>> memoria;
    std::mutex mtx;  

public:
    Disco(); 

    void write(int valor); 
    void display() const;  
    void setRegistersFromFile(Registers& regs, const std::string& regsFilename);
    int loadInstructionsFromFile(RAM& ram, const std::string& instrFilename);
};

#endif
