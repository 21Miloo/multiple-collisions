#include "obstaculo.h"
#include <algorithm>

Obstaculo::Obstaculo(double x, double y, double lado, double e): x(x), y(y), lado(lado), e(e) {}

Lado Obstaculo::detectarLado(const Particula& p) const {

    double px = p.getX();
    double py = p.getY();
    double r = p.getRadio();

    // 1) Punto del cuadrado mas cercano al centro de la particula
    double cx = std::clamp(px, x, x + lado);
    double cy = std::clamp(py, y, y + lado);
    double dx = px - cx;
    double dy = py - cy;

    // Si ese punto esta mas lejos que el radio, no hay contacto
    if (dx * dx + dy * dy > r * r) return Lado::NINGUNO;

    // 2) Cuanto "penetra" la particula por cada lado.
    //    El lado con menor penetracion es por donde entro.
    double penIzq = (px + r) - x;
    double penDer = (x + lado) - (px - r);
    double penAbajo = (py + r) - y;
    double penArriba = (y + lado) - (py - r);

    double minimo = std::min({penIzq, penDer, penAbajo, penArriba});

    if (minimo == penIzq) return Lado::IZQUIERDA;
    if (minimo == penDer) return Lado::DERECHA;
    if (minimo == penAbajo) return Lado::ABAJO;

    return Lado::ARRIBA;
}

bool Obstaculo::rebotar(Particula& p, Lado l) const{

    double vx = p.getVx();
    double vy = p.getVy();
    double px = p.getX();
    double py = p.getY();
    double r = p.getRadio();

    switch (l) {
    case Lado::IZQUIERDA:
        if (vx <= 0) return false;
        vx = -e * vx;
        px = x - r;
        break;

    case Lado::DERECHA:
        if (vx >= 0) return false;
        vx = -e * vx;
        px = x + lado + r;
        break;

    case Lado::ABAJO:
        if (vy <= 0) return false;
        vy = -e * vy;
        py = y - r;
        break;

    case Lado::ARRIBA:
        if (vy >= 0) return false;
        vy = -e * vy;
        py = y + lado + r;
        break;

    case Lado::NINGUNO:
        return false;
    }

    p.setPosicion(px, py);   // se saca la particula del obstaculo
    p.setVelocidad(vx, vy);
    return true;
}
