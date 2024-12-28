#ifndef BOOTLOADER_HPP
#define BOOTLOADER_HPP

#include "Registers.hpp"
#include "RAM.hpp"
#include "Disco.hpp"
#include "ProcessManager.hpp"
#include "Core.hpp"

#include <vector>
#include <memory>

class Bootloader {
    public:
        static void inicializarSistema(Registers& regs, RAM& ram, Disco& disco, ProcessManager& processManager, std::vector<std::unique_ptr<Core>>& cores);
};

#endif