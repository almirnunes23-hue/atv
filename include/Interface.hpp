#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <string>

#include "DateUF.hpp"
#include "Filtro.hpp"
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
    void verRecomendacoes();
    void verMatches();

    void configurarFiltro();
    void mostrarFiltro() const;
    void mostrarPerfil(const Perfil& perfil) const;
    void mostrarContato(const Perfil& perfil) const;
    void escolherCursoFiltro();
    void escolherIdadeFiltro();
    void escolherGeneroFiltro();
    void escolherSemestreFiltro();
    void escolherPreferenciasFiltro();

    bool lerLinha(std::string& linha);
    bool lerTexto(const std::string& pergunta, std::string& texto);
    bool lerInteiro(const std::string& pergunta, int& valor);
    bool lerInteiroNaFaixa(const std::string& pergunta, int minimo,
                           int maximo, int& valor);
    bool lerNome(std::string& nome);
    bool lerCurso(Perfil& perfil);
    bool lerGenero(Perfil& perfil);
    bool lerPreferencias(Perfil& perfil);

    DateUF sistema;
    Filtro filtro;
    int usuarioAtual;
    bool fimDaEntrada;
};

#endif
