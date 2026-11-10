#include "Filtro.hpp"

Filtro::Filtro()
    : idadeMin(0), idadeMax(0), semestreMin(0), semestreMax(0) {
}

void Filtro::limpar() {
    curso = "";
    genero = "";
    idadeMin = 0;
    idadeMax = 0;
    semestreMin = 0;
    semestreMax = 0;
    preferencias.clear();
}

void Filtro::limparPreferencias() {
    preferencias.clear();
}

void Filtro::setCurso(const std::string& curso) {
    this->curso = curso;
}

void Filtro::setIdadeMin(int idade) {
    idadeMin = idade;
}

void Filtro::setIdadeMax(int idade) {
    idadeMax = idade;
}

void Filtro::setGenero(const std::string& genero) {
    this->genero = genero;
}

void Filtro::setSemestreMin(int semestre) {
    semestreMin = semestre;
}

void Filtro::setSemestreMax(int semestre) {
    semestreMax = semestre;
}

void Filtro::adicionarPreferencia(const std::string& preferencia) {
    preferencias.insert(preferencia);
}

std::string Filtro::getCurso() const {
    return curso;
}

int Filtro::getIdadeMin() const {
    return idadeMin;
}

int Filtro::getIdadeMax() const {
    return idadeMax;
}

std::string Filtro::getGenero() const {
    return genero;
}

int Filtro::getSemestreMin() const {
    return semestreMin;
}

int Filtro::getSemestreMax() const {
    return semestreMax;
}

std::set<std::string> Filtro::getPreferencias() const {
    return preferencias;
}

bool Filtro::atende(const Perfil& perfil) const {
    if (!curso.empty() && perfil.getCurso() != curso) {
        return false;
    }
    if (!genero.empty() && perfil.getGenero() != genero) {
        return false;
    }
    if (idadeMin > 0 && perfil.getIdade() < idadeMin) {
        return false;
    }
    if (idadeMax > 0 && perfil.getIdade() > idadeMax) {
        return false;
    }
    if (semestreMin > 0 && perfil.getSemestre() < semestreMin) {
        return false;
    }
    if (semestreMax > 0 && perfil.getSemestre() > semestreMax) {
        return false;
    }

    if (!preferencias.empty()) {
        bool temAlguma = false;
        std::set<std::string>::const_iterator it;

        for (it = preferencias.begin(); it != preferencias.end(); ++it) {
            if (perfil.temPreferencia(*it)) {
                temAlguma = true;
                break;
            }
        }

        if (!temAlguma) {
            return false;
        }
    }

    return true;
}
