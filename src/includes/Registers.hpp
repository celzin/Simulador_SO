#ifndef REGISTERS_HPP
#define REGISTERS_HPP
#define NUM_REGISTRADORES 12

#include <iostream>
#include <vector>

using namespace std;

class Registers {
private: 
    vector<pair<int, bool>> registradores;

public:
    Registers(); 

    std::vector<int> getAll() const;
    int get(int index) const;
    void set(int index, int value);
    void display() const;
};

#endif
