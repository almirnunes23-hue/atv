#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <string>

#include "DateUF.hpp"
#include "Perfil.hpp"

class Interface {
public:
    Interface();
    void executar();

private:
    void mostrarMenu() const;
    int lerOpcao();

    void cadastrarPerfil();
    void entrarComo();
    void listarPerfis();

    bool lerLinha(std::string& linha);
    bool lerTexto(const std::string& pergunta, std::string& texto);
    bool lerInteiro(const std::string& pergunta, int& valor);
    bool lerCurso(Perfil& perfil);
    bool lerGenero(Perfil& perfil);
    bool lerPreferencias(Perfil& perfil);

    DateUF sistema;
    int usuarioAtual;
    bool fimDaEntrada;
};

#endif
