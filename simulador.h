#ifndef SIMULADOR_H
#define SIMULADOR_H

#include <fstream>
#include <string>
#include <vector>

#include "box.h"
#include "obstaculo.h"
#include "particula.h"

class Simulador{

private:

    Box caja;
    std::vector<Particula> particulas;
    std::vector<Obstaculo> obstaculos;
    double dt;
    double t;
    std::ofstream salida;

    void paso();
    void detectarColisionesParticulas();
    void registrarEscena();
    void registrarPosiciones();
    void registrarColision(const std::string& tipo, int id1, int id2);

public:

    Simulador(const Box& caja, double dt, const std::string& archivo);

    void agregarParticula(const Particula& p);
    void agregarObstaculo(const Obstaculo& o);

    // Corre la simulacion nPasos veces, escribiendo todo en el archivo
    void ejecutar(int nPasos);
};

#endif // SIMULADOR_H
