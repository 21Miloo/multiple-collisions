#ifndef PARTICULA_H
#define PARTICULA_H

class Particula{

private:

    int id;
    double x;
    double y;
    double vx;
    double vy;
    double masa;
    double radio;
    bool activa;   // false cuando fue absorbida por otra particula

public:

    Particula(int id, double x, double y, double vx, double vy, double masa, double radio);

    // Movimiento rectilineo uniforme durante dt: x = x0 + vx*dt
    void mover(double dt);

    // true si ambas estan activas y sus circulos se tocan
    bool colisionaCon(const Particula& otra) const;

    // Choque completamente inelastico: esta particula absorbe a "otra"
    // M = m1 + m2,  v' = (m1 v1 + m2 v2) / M
    // posicion = centro de masa, radio tal que el area se conserva
    // "otra" queda inactiva
    void fusionar(Particula& otra);

    // Getters
    int getId() const { return id; }
    double getX() const { return x; }
    double getY() const { return y; }
    double getVx() const { return vx; }
    double getVy() const { return vy; }
    double getMasa() const { return masa; }
    double getRadio() const { return radio; }
    bool estaActiva() const { return activa; }

    // Setters
    void setPosicion(double nx, double ny) { x = nx; y = ny; }
    void setVelocidad(double nvx, double nvy) { vx = nvx; vy = nvy; }
};

#endif // PARTICULA_H
