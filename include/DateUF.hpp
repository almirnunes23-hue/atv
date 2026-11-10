#ifndef DATEUF_HPP
#define DATEUF_HPP

#include <list>
#include <map>
#include <vector>

#include "Curtidas.hpp"
#include "Filtro.hpp"
#include "Perfil.hpp"

class DateUF {
public:
    DateUF();

    int cadastrarPerfil(const Perfil& perfil);
    bool existePerfil(int id) const;
    bool obterPerfil(int id, Perfil& destino) const;
    std::vector<Perfil> listarPerfis() const;

    void montarFila(int idUsuario, const Filtro& filtro);
    bool filaVazia() const;
    bool proximoDaFila(Perfil& destino) const;
    void avancarFila();

    void curtir(int de, int para);
    bool deuMatch(int a, int b) const;
    std::vector<Perfil> listarMatches(int idUsuario) const;

private:
    std::map<int, Perfil> perfis;
    std::list<Perfil> fila;
    Curtidas curtidas;
    int proximoId;
};

#endif
