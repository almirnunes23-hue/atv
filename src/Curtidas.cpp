#include "Curtidas.hpp"

Curtidas::Curtidas() {
}

void Curtidas::curtir(int de, int para) {
    curtidas[de].insert(para);
}

bool Curtidas::jaCurtiu(int de, int para) const {
    std::map<int, std::set<int> >::const_iterator it = curtidas.find(de);

    if (it == curtidas.end()) {
        return false;
    }

    return it->second.count(para) > 0;
}

bool Curtidas::houveMatch(int a, int b) const {
    return jaCurtiu(a, b) && jaCurtiu(b, a);
}
