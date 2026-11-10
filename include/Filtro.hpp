#ifndef FILTRO_HPP
#define FILTRO_HPP

#include <set>
#include <string>

#include "Perfil.hpp"

class Filtro {
public:
    Filtro();

    void limpar();
    void limparPreferencias();

    void setCurso(const std::string& curso);
    void setIdadeMin(int idade);
    void setIdadeMax(int idade);
    void setGenero(const std::string& genero);
    void setSemestreMin(int semestre);
    void setSemestreMax(int semestre);
    void adicionarPreferencia(const std::string& preferencia);

    std::string getCurso() const;
    int getIdadeMin() const;
    int getIdadeMax() const;
    std::string getGenero() const;
    int getSemestreMin() const;
    int getSemestreMax() const;
    std::set<std::string> getPreferencias() const;

    bool atende(const Perfil& perfil) const;

private:
    std::string curso;
    int idadeMin;
    int idadeMax;
    std::string genero;
    int semestreMin;
    int semestreMax;
    std::set<std::string> preferencias;
};

#endif
