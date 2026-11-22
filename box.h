#ifndef BOX_H
#define BOX_H

#include "particula.h"

// Caja rectangular con esquina inferior izquierda en (0, 0)
class Box{

private:

    double ancho;
    double altura;

public:

    // Constructor
    Box(double ancho, double altura);

    // Choque perfectamente elastico: invierte vx o vy segun la pared.
    // Retorna true si hubo rebote.
    bool rebotarParedes(Particula& p) const;

    double getAncho() const { return ancho; }
    double getAltura() const { return altura; }
};

#endif // BOX_H
