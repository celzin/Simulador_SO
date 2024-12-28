#include "includes/Bootloader.hpp"
#include "includes/RAM.hpp"
#include "includes/Disco.hpp"
#include "includes/Registers.hpp"
#include "includes/Core.hpp"

#include <vector>
#include <memory>

int main() {
    RAM ram;
    Disco disco;
    Registers regs;
    std::vector<Core*> cores;
    
    Bootloader::inicializarSistema(regs, ram, disco, cores);

    return 0;
}
