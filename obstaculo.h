#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "particula.h"

// Lado del cuadrado contra el que choco la particula
enum class Lado { NINGUNO, IZQUIERDA, DERECHA, ARRIBA, ABAJO };

// Obstaculo cuadrado estatico. (x, y) es su esquina inferior izquierda.
class Obstaculo{

private:

    double x;
    double y;
    double lado;
    double e; // coeficiente de restitucion (0 < e < 1)

public:

    Obstaculo(double x, double y, double lado, double e);

    // Retorna con que lado del cuadrado esta chocando la particula
    Lado detectarLado(const Particula& p) const;

    // Choque inelastico: v_perp' = -e * v_perp,  v_par' = v_par
    // Retorna true si hubo rebote
    bool rebotar(Particula& p, Lado l) const;

    double getX() const { return x; }
    double getY() const { return y; }
    double getLado() const { return lado; }
    double getE() const { return e; }
};

#endif // OBSTACULO_H
