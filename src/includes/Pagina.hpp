#ifndef PAGINA_HPP
#define PAGINA_HPP

#include <vector>

class Pagina {
public:
    int id;                         // Identificador da página
    bool emUso;                     // Se a página está em uso
    std::vector<int> dados;         // Dados armazenados na página
    static const int TAMANHO = 4;   // Tamanho fixo da página

    Pagina(int id);
};

#endif
