#include "particula.h"
#include <cmath>

Particula::Particula(int id, double x, double y, double vx, double vy, double masa, double radio)
    : id(id), x(x), y(y), vx(vx), vy(vy), masa(masa), radio(radio), activa(true) {}

void Particula::mover(double dt)
{
    x += vx * dt;
    y += vy * dt;
}

bool Particula::colisionaCon(const Particula& otra) const
{
    if (!activa || !otra.activa || id == otra.id) return false;

    double dx = otra.x - x;
    double dy = otra.y - y;
    double sumaRadios = radio + otra.radio;

    // Se compara con distancias al cuadrado para evitar la raiz
    return dx * dx + dy * dy <= sumaRadios * sumaRadios;
}

void Particula::fusionar(Particula& otra)
{
    double M = masa + otra.masa;

    // Conservacion del momento lineal
    double nvx = (masa * vx + otra.masa * otra.vx) / M;
    double nvy = (masa * vy + otra.masa * otra.vy) / M;

    // Centro de masa
    double nx = (masa * x + otra.masa * otra.x) / M;
    double ny = (masa * y + otra.masa * otra.y) / M;

    // Se conserva el area: pi*R^2 = pi*r1^2 + pi*r2^2
    double nr = std::sqrt(radio * radio + otra.radio * otra.radio);

    x = nx;  y = ny;
    vx = nvx; vy = nvy;
    masa = M;
    radio = nr;

    otra.activa = false;
}
