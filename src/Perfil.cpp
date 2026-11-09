#include "Perfil.hpp"

Perfil::Perfil()
    : id(0), idade(0), semestre(0) {
}

Perfil::Perfil(int id, const std::string& nome)
    : id(id), nome(nome), idade(0), semestre(0) {
}

int Perfil::getId() const {
    return id;
}

void Perfil::setId(int id) {
    this->id = id;
}

std::string Perfil::getNome() const {
    return nome;
}

void Perfil::setNome(const std::string& nome) {
    this->nome = nome;
}

std::string Perfil::getCurso() const {
    return curso;
}

void Perfil::setCurso(const std::string& curso) {
    this->curso = curso;
}

int Perfil::getIdade() const {
    return idade;
}

void Perfil::setIdade(int idade) {
    this->idade = idade;
}

std::string Perfil::getGenero() const {
    return genero;
}

void Perfil::setGenero(const std::string& genero) {
    this->genero = genero;
}

int Perfil::getSemestre() const {
    return semestre;
}

void Perfil::setSemestre(int semestre) {
    this->semestre = semestre;
}

std::set<std::string> Perfil::getPreferencias() const {
    return preferencias;
}

void Perfil::adicionarPreferencia(const std::string& preferencia) {
    preferencias.insert(preferencia);
}

bool Perfil::temPreferencia(const std::string& preferencia) const {
    return preferencias.count(preferencia) > 0;
}

std::string Perfil::getWhatsapp() const {
    return whatsapp;
}

void Perfil::setWhatsapp(const std::string& whatsapp) {
    this->whatsapp = whatsapp;
}

std::string Perfil::getTelegram() const {
    return telegram;
}

void Perfil::setTelegram(const std::string& telegram) {
    this->telegram = telegram;
}

bool Perfil::maiorDeIdade() const {
    return idade >= 18;
}

bool Perfil::operator<(const Perfil& outro) const {
    if (nome != outro.nome) {
        return nome < outro.nome;
    }
    return id < outro.id;
}

bool Perfil::operator==(const Perfil& outro) const {
    return id == outro.id;
}
