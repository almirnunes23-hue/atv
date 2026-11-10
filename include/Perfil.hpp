#ifndef PERFIL_HPP
#define PERFIL_HPP

#include <set>
#include <string>

class Perfil {
public:
    Perfil();
    Perfil(int id, const std::string& nome);

    int getId() const;
    void setId(int id);

    std::string getNome() const;
    void setNome(const std::string& nome);

    std::string getCurso() const;
    void setCurso(const std::string& curso);

    int getIdade() const;
    void setIdade(int idade);

    std::string getGenero() const;
    void setGenero(const std::string& genero);

    int getSemestre() const;
    void setSemestre(int semestre);

    std::set<std::string> getPreferencias() const;
    void adicionarPreferencia(const std::string& preferencia);
    bool temPreferencia(const std::string& preferencia) const;

    std::string getContato() const;
    void setContato(const std::string& contato);

    bool maiorDeIdade() const;

    bool operator<(const Perfil& outro) const;
    bool operator==(const Perfil& outro) const;

private:
    int id;
    std::string nome;
    std::string curso;
    int idade;
    std::string genero;
    int semestre;
    std::set<std::string> preferencias;
    std::string contato;
};

#endif
