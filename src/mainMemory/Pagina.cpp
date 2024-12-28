#include "../includes/Pagina.hpp"

Pagina::Pagina(int id) : id(id), emUso(false) {
    dados.resize(TAMANHO, 0); // Inicializa a página com valores 0
}
