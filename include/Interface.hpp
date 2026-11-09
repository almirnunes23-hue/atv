#ifndef INTERFACE_HPP
#define INTERFACE_HPP

class Interface {
public:
    Interface();
    void executar();

private:
    void mostrarMenu() const;
    int lerOpcao() const;
};

#endif
