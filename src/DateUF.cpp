#include "DateUF.hpp"

#include <algorithm>

DateUF::DateUF()
    : proximoId(1) {
}

int DateUF::cadastrarPerfil(const Perfil& perfil) {
    int id = proximoId;
    ++proximoId;

    Perfil copia = perfil;
    copia.setId(id);
    perfis[id] = copia;

    return id;
}

bool DateUF::existePerfil(int id) const {
    return perfis.count(id) > 0;
}

bool DateUF::obterPerfil(int id, Perfil& destino) const {
    std::map<int, Perfil>::const_iterator it = perfis.find(id);

    if (it == perfis.end()) {
        return false;
    }

    destino = it->second;
    return true;
}

std::vector<Perfil> DateUF::listarPerfis() const {
    std::vector<Perfil> lista;
    std::map<int, Perfil>::const_iterator it;

    for (it = perfis.begin(); it != perfis.end(); ++it) {
        lista.push_back(it->second);
    }

    std::sort(lista.begin(), lista.end());
    return lista;
}

void DateUF::montarFila(int idUsuario, const Filtro& filtro) {
    fila.clear();

    std::map<int, Perfil>::const_iterator it;
    for (it = perfis.begin(); it != perfis.end(); ++it) {
        if (it->first == idUsuario) {
            continue;
        }
        if (curtidas.jaCurtiu(idUsuario, it->first)) {
            continue;
        }
        if (filtro.atende(it->second)) {
            fila.push_back(it->second);
        }
    }

    fila.sort();
}

bool DateUF::filaVazia() const {
    return fila.empty();
}

bool DateUF::proximoDaFila(Perfil& destino) const {
    if (fila.empty()) {
        return false;
    }

    destino = fila.front();
    return true;
}

void DateUF::avancarFila() {
    if (!fila.empty()) {
        fila.pop_front();
    }
}

void DateUF::curtir(int de, int para) {
    curtidas.curtir(de, para);
}

bool DateUF::deuMatch(int a, int b) const {
    return curtidas.houveMatch(a, b);
}

std::vector<Perfil> DateUF::listarMatches(int idUsuario) const {
    std::vector<Perfil> matches;
    std::map<int, Perfil>::const_iterator it;

    for (it = perfis.begin(); it != perfis.end(); ++it) {
        if (it->first == idUsuario) {
            continue;
        }
        if (curtidas.houveMatch(idUsuario, it->first)) {
            matches.push_back(it->second);
        }
    }

    std::sort(matches.begin(), matches.end());
    return matches;
}
