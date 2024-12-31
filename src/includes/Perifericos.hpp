#ifndef PERIFERICOS_HPP
#define PERIFERICOS_HPP

#include <unordered_map>
#include <string>
#include <iostream>

using namespace std;

class Perifericos {
private:
    std::unordered_map<std::string, bool> perifericos; 

public:
    Perifericos();

    void estadoPeriferico(const std::string& nomeRecurso, bool estado); 
    bool verificarPeriferico(const std::string& nomeRecurso) const;     

    void exibirPerifericos() const; 
};

#endif
