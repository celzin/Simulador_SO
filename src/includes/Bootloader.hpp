#ifndef BOOTLOADER_HPP
#define BOOTLOADER_HPP

#include "Registers.hpp"
#include "RAM.hpp"
#include "Disco.hpp"
#include "Core.hpp"

#include <vector>

class Bootloader {
    public:
        static void inicializarSistema(Registers& regs, RAM& ram, Disco& disco, std::vector<Core*>& cores);
};

#endif