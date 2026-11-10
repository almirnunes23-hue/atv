#include "Interface.hpp"

#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

static const int TOTAL_CURSOS = 6;
static const char* CURSOS[TOTAL_CURSOS] = {
    "Administracao",
    "Ciencia da Computacao",
    "Direito",
    "Engenharia Civil",
    "Medicina",
    "Psicologia"
};

static const int TOTAL_GENEROS = 3;
static const char* GENEROS[TOTAL_GENEROS] = {
    "Feminino",
    "Masculino",
    "Outro"
};

static const int TOTAL_PREFERENCIAS = 6;
static const char* PREFERENCIAS[TOTAL_PREFERENCIAS] = {
    "atividade fisica",
    "beber",
    "videogames",
    "animes",
    "esportes",
    "leitura"
};

Interface::Interface()
    : usuarioAtual(-1), fimDaEntrada(false) {
}

void Interface::executar() {
    while (!fimDaEntrada) {
        mostrarMenu();
        int opcao = lerOpcao();

        if (fimDaEntrada) {
            break;
        }

        if (opcao == 0) {
            break;
        } else if (opcao == 1) {
            cadastrarPerfil();
        } else if (opcao == 2) {
            entrarComo();
        } else if (opcao == 3) {
            listarPerfis();
        } else {
            std::cout << "Opcao invalida." << std::endl;
        }
    }

    std::cout << "Ate logo!" << std::endl;
}

void Interface::mostrarMenu() const {
    std::cout << std::endl;
    std::cout << "=== dateUF ===" << std::endl;

    if (usuarioAtual >= 0) {
        std::cout << "Usuario atual: perfil " << usuarioAtual << std::endl;
    } else {
        std::cout << "Nenhum usuario conectado." << std::endl;
    }

    std::cout << "1 - Cadastrar perfil" << std::endl;
    std::cout << "2 - Entrar como um perfil" << std::endl;
    std::cout << "3 - Listar perfis" << std::endl;
    std::cout << "0 - Sair" << std::endl;
    std::cout << "Opcao: ";
}

int Interface::lerOpcao() {
    std::string linha;

    if (!lerLinha(linha)) {
        return -1;
    }

    std::stringstream ss(linha);
    int valor = 0;
    char resto = 0;

    if (!(ss >> valor)) {
        return -1;
    }
    if (ss >> resto) {
        return -1;
    }

    return valor;
}

void Interface::cadastrarPerfil() {
    std::cout << std::endl;
    std::cout << "--- Cadastrar perfil ---" << std::endl;

    std::string nome;
    if (!lerNome(nome)) {
        return;
    }

    Perfil perfil;
    perfil.setNome(nome);

    if (!lerCurso(perfil)) {
        return;
    }

    int idade = 0;
    if (!lerInteiroNaFaixa("Idade (18 a 100): ", 18, 100, idade)) {
        return;
    }
    perfil.setIdade(idade);

    if (!lerGenero(perfil)) {
        return;
    }

    int semestre = 0;
    if (!lerInteiroNaFaixa("Semestre (1 a 12): ", 1, 12, semestre)) {
        return;
    }
    perfil.setSemestre(semestre);

    if (!lerPreferencias(perfil)) {
        return;
    }

    std::string contato;
    if (!lerTexto("Contato (WhatsApp ou Telegram, pode ficar vazio): ",
                  contato)) {
        return;
    }
    perfil.setContato(contato);

    int id = sistema.cadastrarPerfil(perfil);
    std::cout << "Perfil cadastrado com o numero " << id << "." << std::endl;
}

void Interface::entrarComo() {
    int id = 0;

    if (!lerInteiro("Numero do perfil: ", id)) {
        if (!fimDaEntrada) {
            std::cout << "Erro: numero invalido." << std::endl;
        }
        return;
    }

    Perfil perfil;
    if (!sistema.obterPerfil(id, perfil)) {
        std::cout << "Erro: perfil " << id << " nao existe." << std::endl;
        return;
    }

    usuarioAtual = id;
    std::cout << "Voce entrou como " << perfil.getNome()
              << " (perfil " << id << ")." << std::endl;
}

void Interface::listarPerfis() {
    std::vector<Perfil> perfis = sistema.listarPerfis();

    if (perfis.empty()) {
        std::cout << "Nenhum perfil cadastrado." << std::endl;
        return;
    }

    for (std::size_t i = 0; i < perfis.size(); ++i) {
        std::cout << perfis[i].getId() << " - " << perfis[i].getNome()
                  << " | curso: " << perfis[i].getCurso()
                  << " | idade: " << perfis[i].getIdade()
                  << " | genero: " << perfis[i].getGenero()
                  << " | semestre: " << perfis[i].getSemestre()
                  << std::endl;
    }
}

bool Interface::lerLinha(std::string& linha) {
    if (!std::getline(std::cin, linha)) {
        fimDaEntrada = true;
        return false;
    }
    return true;
}

bool Interface::lerTexto(const std::string& pergunta, std::string& texto) {
    std::cout << pergunta;
    return lerLinha(texto);
}

bool Interface::lerInteiro(const std::string& pergunta, int& valor) {
    std::string linha;

    std::cout << pergunta;
    if (!lerLinha(linha)) {
        return false;
    }

    std::stringstream ss(linha);
    char resto = 0;

    if (!(ss >> valor)) {
        return false;
    }
    if (ss >> resto) {
        return false;
    }

    return true;
}

bool Interface::lerInteiroNaFaixa(const std::string& pergunta, int minimo,
                                  int maximo, int& valor) {
    while (true) {
        std::string linha;

        std::cout << pergunta;
        if (!lerLinha(linha)) {
            return false;
        }

        std::stringstream ss(linha);
        int lido = 0;
        char resto = 0;

        if ((ss >> lido) && !(ss >> resto) && lido >= minimo
            && lido <= maximo) {
            valor = lido;
            return true;
        }

        std::cout << "Opcao invalida." << std::endl;
    }
}

bool Interface::lerNome(std::string& nome) {
    while (true) {
        std::cout << "Nome: ";
        if (!lerLinha(nome)) {
            return false;
        }
        if (!nome.empty()) {
            return true;
        }
        std::cout << "Opcao invalida." << std::endl;
    }
}

bool Interface::lerCurso(Perfil& perfil) {
    std::cout << "Curso:" << std::endl;
    for (int i = 0; i < TOTAL_CURSOS; ++i) {
        std::cout << "  " << (i + 1) << " - " << CURSOS[i] << std::endl;
    }

    int opcao = 0;
    if (!lerInteiroNaFaixa("Opcao: ", 1, TOTAL_CURSOS, opcao)) {
        return false;
    }

    perfil.setCurso(CURSOS[opcao - 1]);
    return true;
}

bool Interface::lerGenero(Perfil& perfil) {
    std::cout << "Genero:" << std::endl;
    for (int i = 0; i < TOTAL_GENEROS; ++i) {
        std::cout << "  " << (i + 1) << " - " << GENEROS[i] << std::endl;
    }

    int opcao = 0;
    if (!lerInteiroNaFaixa("Opcao: ", 1, TOTAL_GENEROS, opcao)) {
        return false;
    }

    perfil.setGenero(GENEROS[opcao - 1]);
    return true;
}

bool Interface::lerPreferencias(Perfil& perfil) {
    std::cout << "Preferencias (numeros separados por espaco,"
              << " Enter para nenhuma):" << std::endl;
    for (int i = 0; i < TOTAL_PREFERENCIAS; ++i) {
        std::cout << "  " << (i + 1) << " - " << PREFERENCIAS[i] << std::endl;
    }

    while (true) {
        std::string linha;
        std::cout << "Opcoes: ";
        if (!lerLinha(linha)) {
            return false;
        }

        std::stringstream ss(linha);
        std::vector<int> escolhidas;
        int numero = 0;
        bool valido = true;

        while (ss >> numero) {
            if (numero < 1 || numero > TOTAL_PREFERENCIAS) {
                valido = false;
                break;
            }
            escolhidas.push_back(numero);
        }

        if (valido && !ss.eof()) {
            valido = false;
        }

        if (!valido) {
            std::cout << "Opcao invalida." << std::endl;
            continue;
        }

        for (std::size_t i = 0; i < escolhidas.size(); ++i) {
            perfil.adicionarPreferencia(PREFERENCIAS[escolhidas[i] - 1]);
        }

        return true;
    }
}
