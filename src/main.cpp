#include "includes/Bootloader.hpp"
#include "includes/RAM.hpp"
#include "includes/Disco.hpp"
#include "includes/ProcessManager.hpp"
#include "includes/Core.hpp"

#include <vector>
#include <memory>

int main() {
    RAM ram;
    Disco disco;
    ProcessManager processManager;
    std::vector<std::unique_ptr<Core>> cores;
    
    Bootloader::inicializarSistema(ram, disco, processManager, cores);

    return 0;
}
