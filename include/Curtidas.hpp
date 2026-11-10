#ifndef CURTIDAS_HPP
#define CURTIDAS_HPP

#include <map>
#include <set>

class Curtidas {
public:
    Curtidas();

    void curtir(int de, int para);
    bool jaCurtiu(int de, int para) const;
    bool houveMatch(int a, int b) const;

private:
    std::map<int, std::set<int> > curtidas;
};

#endif
