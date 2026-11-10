#ifndef DATEUF_HPP
#define DATEUF_HPP

#include <map>
#include <vector>

#include "Perfil.hpp"

class DateUF {
public:
    DateUF();

    int cadastrarPerfil(const Perfil& perfil);
    bool existePerfil(int id) const;
    bool obterPerfil(int id, Perfil& destino) const;
    std::vector<Perfil> listarPerfis() const;

private:
    std::map<int, Perfil> perfis;
    int proximoId;
};

#endif
