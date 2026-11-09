#include "Interface.hpp"

#include <iostream>
#include <sstream>
#include <string>

Interface::Interface() {
}

void Interface::executar() {
    bool sair = false;

    while (!sair) {
        mostrarMenu();
        int opcao = lerOpcao();

        if (opcao == 0) {
            sair = true;
        } else {
            std::cout << "Opção inválida." << std::endl;
        }
    }

    std::cout << "Até logo!" << std::endl;
}

void Interface::mostrarMenu() const {
    std::cout << std::endl;
    std::cout << "=== dateUF ===" << std::endl;
    std::cout << "0 - Sair" << std::endl;
    std::cout << "Opção: ";
}

int Interface::lerOpcao() const {
    std::string linha;

    if (!std::getline(std::cin, linha)) {
        return 0;
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
