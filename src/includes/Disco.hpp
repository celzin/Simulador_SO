#ifndef DISCO_HPP
#define DISCO_HPP

#include "Registers.hpp"
#include "RAM.hpp"

#include <filesystem>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>


using namespace std;

class Disco {
private:
    static const int LINHAS = 10;
    static const int COLUNAS = 10;
    
    vector<vector<pair<int, bool>>> memoria;

    vector<string> listInstructionsFiles(const string& listDir, const string& extension);

public:
    Disco(); 

    void write(int valor); 
    void display() const;
    void setRegistersFromFile(Registers& regs, const string& regsFilename);
    int loadInstructionsFromFile(RAM& ram, const string& instrFilename, int baseAddress);
    vector<pair<string, int>> loadProcessFromFiles(RAM& ram, const string& diretorio);
};

#endif