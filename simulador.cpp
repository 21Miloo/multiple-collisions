#include "simulador.h"
#include <iostream>

Simulador::Simulador(const Box& caja, double dt, const std::string& archivo): caja(caja), dt(dt), t(0.0), salida(archivo)
{
    if (!salida.is_open())
        std::cerr << "No se pudo abrir el archivo " << archivo << std::endl;
}

void Simulador::agregarParticula(const Particula& p){
    particulas.push_back(p);
}

void Simulador::agregarObstaculo(const Obstaculo& o){
    obstaculos.push_back(o);
}

void Simulador::ejecutar(int nPasos){
    registrarEscena();
    registrarPosiciones();          // estado inicial, t = 0

    for (int i = 0; i < nPasos; i++) {
        paso();
    }
}

void Simulador::paso(){
    t += dt;

    for (Particula& p : particulas) {
        if (!p.estaActiva()) continue;

        // 1) Mover
        p.mover(dt);

        // 2) Paredes (elastico)
        if (caja.rebotarParedes(p))
            registrarColision("PARED", p.getId(), -1);

        // 3) Obstaculos (inelastico, coef. e)
        for (size_t k = 0; k < obstaculos.size(); k++) {
            Lado l = obstaculos[k].detectarLado(p);
            if (l != Lado::NINGUNO && obstaculos[k].rebotar(p, l))
                registrarColision("OBSTACULO", p.getId(), static_cast<int>(k));
        }
    }

    // 4) Entre particulas (completamente inelastico)
    detectarColisionesParticulas();

    registrarPosiciones();
}

void Simulador::detectarColisionesParticulas(){
    for (size_t i = 0; i < particulas.size(); i++) {
        for (size_t j = i + 1; j < particulas.size(); j++) {
            if (particulas[i].colisionaCon(particulas[j])) {
                Particula& a = particulas[i];
                Particula& b = particulas[j];
                registrarColision("PARTICULA", a.getId(), b.getId());

                // Momento total antes: m1*v1 + m2*v2
                double pxAntes = a.getMasa() * a.getVx() + b.getMasa() * b.getVx();
                double pyAntes = a.getMasa() * a.getVy() + b.getMasa() * b.getVy();

                // a absorbe a b; b queda inactiva y ya no se mueve ni se registra
                a.fusionar(b);

                // Momento total despues: (m1 + m2) * v'
                double pxDespues = a.getMasa() * a.getVx();
                double pyDespues = a.getMasa() * a.getVy();

                std::cout << "t = " << t << " s  fusion " << a.getId() << " + " << b.getId()
                          << "  |  p antes = (" << pxAntes << ", " << pyAntes << ")"
                          << "  p despues = (" << pxDespues << ", " << pyDespues << ")"
                          << "  ->  diferencia = (" << pxDespues - pxAntes << ", "
                          << pyDespues - pyAntes << ")" << std::endl;
            }
        }
    }
}

// ---------------- Archivo de salida ----------------
// Formato (separado por espacios, una linea por registro):
//   CAJA ancho altura
//   OBS  k x y lado e
//   POS  t id x y radio
//   COL  t tipo id1 id2     (id2 = indice de obstaculo, id de particula o -1 si es pared)

void Simulador::registrarEscena(){

    salida << "# CAJA ancho altura\n";
    salida << "CAJA " << caja.getAncho() << " " << caja.getAltura() << "\n";
    salida << "# OBS k x y lado e\n";
    for (size_t k = 0; k < obstaculos.size(); k++) {
        const Obstaculo& o = obstaculos[k];
        salida << "OBS " << k << " " << o.getX() << " " << o.getY() << " "
               << o.getLado() << " " << o.getE() << "\n";
    }
    salida << "# POS t id x y radio\n";
    salida << "# COL t tipo id1 id2\n";
}

void Simulador::registrarPosiciones() {
    for (const Particula& p : particulas) {
        if (!p.estaActiva()) continue;
        salida << "POS " << t << " " << p.getId() << " "
               << p.getX() << " " << p.getY() << " " << p.getRadio() << "\n";
    }
}

void Simulador::registrarColision(const std::string& tipo, int id1, int id2) {
    salida << "COL " << t << " " << tipo << " " << id1 << " " << id2 << "\n";
}
