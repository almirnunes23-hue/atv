#include "Interface.hpp"

#include <cstddef>
#include <iostream>
#include <set>
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
        } else if (opcao == 4) {
            verRecomendacoes();
        } else if (opcao == 5) {
            configurarFiltro();
        } else if (opcao == 6) {
            verMatches();
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
    std::cout << "4 - Ver recomendacoes" << std::endl;
    std::cout << "5 - Configurar filtro" << std::endl;
    std::cout << "6 - Ver matches" << std::endl;
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

void Interface::verRecomendacoes() {
    if (usuarioAtual < 0) {
        std::cout << "Entre como um perfil primeiro." << std::endl;
        return;
    }

    sistema.montarFila(usuarioAtual, filtro);

    while (!fimDaEntrada) {
        Perfil proximo;
        if (!sistema.proximoDaFila(proximo)) {
            std::cout << "Sem mais recomendacoes por enquanto." << std::endl;
            return;
        }

        mostrarPerfil(proximo);

        std::cout << "1 - Curtir" << std::endl;
        std::cout << "2 - Pular" << std::endl;
        std::cout << "3 - Mudar filtro" << std::endl;
        std::cout << "0 - Voltar" << std::endl;
        std::cout << "Opcao: ";

        int opcao = lerOpcao();
        if (fimDaEntrada) {
            return;
        }

        if (opcao == 0) {
            return;
        } else if (opcao == 1) {
            sistema.curtir(usuarioAtual, proximo.getId());
            std::cout << "Curtida registrada." << std::endl;

            if (sistema.deuMatch(usuarioAtual, proximo.getId())) {
                Perfil eu;
                sistema.obterPerfil(usuarioAtual, eu);

                std::cout << "Deu match com " << proximo.getNome()
                          << "!" << std::endl;
                std::cout << "Seu contato: ";
                mostrarContato(eu);
                std::cout << "Contato de " << proximo.getNome() << ": ";
                mostrarContato(proximo);
            }

            sistema.avancarFila();
        } else if (opcao == 2) {
            sistema.avancarFila();
        } else if (opcao == 3) {
            configurarFiltro();
            sistema.montarFila(usuarioAtual, filtro);
        } else {
            std::cout << "Opcao invalida." << std::endl;
        }
    }
}

void Interface::verMatches() {
    if (usuarioAtual < 0) {
        std::cout << "Entre como um perfil primeiro." << std::endl;
        return;
    }

    std::vector<Perfil> matches = sistema.listarMatches(usuarioAtual);

    if (matches.empty()) {
        std::cout << "Voce ainda nao tem matches." << std::endl;
        return;
    }

    std::cout << "Seus matches:" << std::endl;
    for (std::size_t i = 0; i < matches.size(); ++i) {
        std::cout << matches[i].getId() << " - " << matches[i].getNome()
                  << " | contato: ";
        mostrarContato(matches[i]);
    }
}

void Interface::configurarFiltro() {
    while (!fimDaEntrada) {
        mostrarFiltro();

        std::cout << "1 - Curso" << std::endl;
        std::cout << "2 - Idade" << std::endl;
        std::cout << "3 - Genero" << std::endl;
        std::cout << "4 - Semestre" << std::endl;
        std::cout << "5 - Preferencias" << std::endl;
        std::cout << "6 - Limpar filtro" << std::endl;
        std::cout << "0 - Voltar" << std::endl;
        std::cout << "Opcao: ";

        int opcao = lerOpcao();
        if (fimDaEntrada) {
            return;
        }

        if (opcao == 0) {
            return;
        } else if (opcao == 1) {
            escolherCursoFiltro();
        } else if (opcao == 2) {
            escolherIdadeFiltro();
        } else if (opcao == 3) {
            escolherGeneroFiltro();
        } else if (opcao == 4) {
            escolherSemestreFiltro();
        } else if (opcao == 5) {
            escolherPreferenciasFiltro();
        } else if (opcao == 6) {
            filtro.limpar();
            std::cout << "Filtro limpo." << std::endl;
        } else {
            std::cout << "Opcao invalida." << std::endl;
        }
    }
}

void Interface::mostrarFiltro() const {
    std::cout << std::endl;
    std::cout << "Filtro atual:" << std::endl;

    std::cout << "  Curso: ";
    if (filtro.getCurso().empty()) {
        std::cout << "qualquer";
    } else {
        std::cout << filtro.getCurso();
    }
    std::cout << std::endl;

    std::cout << "  Idade: ";
    if (filtro.getIdadeMin() == 0 && filtro.getIdadeMax() == 0) {
        std::cout << "qualquer";
    } else {
        if (filtro.getIdadeMin() > 0) {
            std::cout << "de " << filtro.getIdadeMin();
        }
        if (filtro.getIdadeMax() > 0) {
            if (filtro.getIdadeMin() > 0) {
                std::cout << " ";
            }
            std::cout << "ate " << filtro.getIdadeMax();
        }
    }
    std::cout << std::endl;

    std::cout << "  Genero: ";
    if (filtro.getGenero().empty()) {
        std::cout << "qualquer";
    } else {
        std::cout << filtro.getGenero();
    }
    std::cout << std::endl;

    std::cout << "  Semestre: ";
    if (filtro.getSemestreMin() == 0 && filtro.getSemestreMax() == 0) {
        std::cout << "qualquer";
    } else {
        if (filtro.getSemestreMin() > 0) {
            std::cout << "de " << filtro.getSemestreMin();
        }
        if (filtro.getSemestreMax() > 0) {
            if (filtro.getSemestreMin() > 0) {
                std::cout << " ";
            }
            std::cout << "ate " << filtro.getSemestreMax();
        }
    }
    std::cout << std::endl;

    std::cout << "  Preferencias:";
    std::set<std::string> prefs = filtro.getPreferencias();
    if (prefs.empty()) {
        std::cout << " qualquer";
    } else {
        std::set<std::string>::const_iterator it;
        for (it = prefs.begin(); it != prefs.end(); ++it) {
            std::cout << " " << *it;
        }
    }
    std::cout << std::endl;
}

void Interface::mostrarPerfil(const Perfil& perfil) const {
    std::cout << std::endl;
    std::cout << "Perfil " << perfil.getId() << ": "
              << perfil.getNome() << std::endl;
    std::cout << "  Curso: " << perfil.getCurso() << std::endl;
    std::cout << "  Idade: " << perfil.getIdade() << std::endl;
    std::cout << "  Genero: " << perfil.getGenero() << std::endl;
    std::cout << "  Semestre: " << perfil.getSemestre() << std::endl;

    std::cout << "  Preferencias:";
    std::set<std::string> prefs = perfil.getPreferencias();
    if (prefs.empty()) {
        std::cout << " nenhuma";
    } else {
        std::set<std::string>::const_iterator it;
        for (it = prefs.begin(); it != prefs.end(); ++it) {
            std::cout << " " << *it;
        }
    }
    std::cout << std::endl;
}

void Interface::mostrarContato(const Perfil& perfil) const {
    if (perfil.getContato().empty()) {
        std::cout << "(nao informado)" << std::endl;
    } else {
        std::cout << perfil.getContato() << std::endl;
    }
}

void Interface::escolherCursoFiltro() {
    std::cout << "Curso (0 = qualquer):" << std::endl;
    for (int i = 0; i < TOTAL_CURSOS; ++i) {
        std::cout << "  " << (i + 1) << " - " << CURSOS[i] << std::endl;
    }

    int opcao = 0;
    if (!lerInteiroNaFaixa("Opcao: ", 0, TOTAL_CURSOS, opcao)) {
        return;
    }

    if (opcao == 0) {
        filtro.setCurso("");
    } else {
        filtro.setCurso(CURSOS[opcao - 1]);
    }
}

void Interface::escolherIdadeFiltro() {
    int minimo = 0;
    int maximo = 0;

    if (!lerInteiroNaFaixa("Idade minima (0 = sem limite): ", 0, 100,
                           minimo)) {
        return;
    }
    if (!lerInteiroNaFaixa("Idade maxima (0 = sem limite): ", 0, 100,
                           maximo)) {
        return;
    }

    if (minimo > 0 && maximo > 0 && minimo > maximo) {
        std::cout << "Opcao invalida." << std::endl;
        return;
    }

    filtro.setIdadeMin(minimo);
    filtro.setIdadeMax(maximo);
}

void Interface::escolherGeneroFiltro() {
    std::cout << "Genero (0 = qualquer):" << std::endl;
    for (int i = 0; i < TOTAL_GENEROS; ++i) {
        std::cout << "  " << (i + 1) << " - " << GENEROS[i] << std::endl;
    }

    int opcao = 0;
    if (!lerInteiroNaFaixa("Opcao: ", 0, TOTAL_GENEROS, opcao)) {
        return;
    }

    if (opcao == 0) {
        filtro.setGenero("");
    } else {
        filtro.setGenero(GENEROS[opcao - 1]);
    }
}

void Interface::escolherSemestreFiltro() {
    int minimo = 0;
    int maximo = 0;

    if (!lerInteiroNaFaixa("Semestre minimo (0 = sem limite): ", 0, 12,
                           minimo)) {
        return;
    }
    if (!lerInteiroNaFaixa("Semestre maximo (0 = sem limite): ", 0, 12,
                           maximo)) {
        return;
    }

    if (minimo > 0 && maximo > 0 && minimo > maximo) {
        std::cout << "Opcao invalida." << std::endl;
        return;
    }

    filtro.setSemestreMin(minimo);
    filtro.setSemestreMax(maximo);
}

void Interface::escolherPreferenciasFiltro() {
    std::cout << "Preferencias (numeros separados por espaco,"
              << " Enter para qualquer):" << std::endl;
    for (int i = 0; i < TOTAL_PREFERENCIAS; ++i) {
        std::cout << "  " << (i + 1) << " - " << PREFERENCIAS[i] << std::endl;
    }

    while (true) {
        std::string linha;
        std::cout << "Opcoes: ";
        if (!lerLinha(linha)) {
            return;
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

        filtro.limparPreferencias();
        for (std::size_t i = 0; i < escolhidas.size(); ++i) {
            filtro.adicionarPreferencia(PREFERENCIAS[escolhidas[i] - 1]);
        }
        return;
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
