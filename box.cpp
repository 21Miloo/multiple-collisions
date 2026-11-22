#include "box.h"

Box::Box(double ancho, double altura) : ancho(ancho), altura(altura) {}

bool Box::rebotarParedes(Particula& p) const {

    double x = p.getX(); // Obtenemos las posicion de la particula en X
    double y = p.getY(); // Obtenemos las posicion de la particula en Y
    double vx = p.getVx(); // Obtenemos la velocidad de la particula en X
    double vy = p.getVy(); // Obtenemos las velocidad de la particula en Y
    double r = p.getRadio(); // obtenemos el radio
    bool reboto = false; // Variable bandera que se vuelve true si hubo rebote

    // Pared izquierda / derecha: se invierte vx.
    // Solo si la particula va hacia la pared (evita que rebote dos veces seguidas).
    // Ademas se corrige la posicion para que no quede por fuera de la caja.
    if (x - r < 0 && vx < 0){

        vx = -vx;
        x = r;
        reboto = true;

    } else if (x + r > ancho && vx > 0){

        vx = -vx;
        x = ancho - r;
        reboto = true;
    }

    // Pared inferior / superior: se invierte vy
    if (y - r < 0 && vy < 0){

        vy = -vy;
        y = r;
        reboto = true;

    } else if (y + r > altura && vy > 0){

        vy = -vy;
        y = altura - r;
        reboto = true;
    }

    p.setPosicion(x, y);
    p.setVelocidad(vx, vy);
    return reboto;
}
