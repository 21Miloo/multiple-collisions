#include <iostream>
#include "simulador.h"

using namespace std;

int main(){

    // Constantes
    const double dt = 0.01;      // s (delta de tiempo)
    const int nPasos = 3000;     // 30 s de simulacion
    const double e = 0.7;        // coeficiente de restitucion de los obstaculos

    Simulador sim(Box(200, 100), dt, "simulacion.txt");

    // 4 obstaculos cuadrados: (x, y) esquina inferior izquierda, lado, e
    sim.agregarObstaculo(Obstaculo( 40, 20, 15, e));
    sim.agregarObstaculo(Obstaculo(140, 20, 15, e));
    sim.agregarObstaculo(Obstaculo( 40, 65, 15, e));
    sim.agregarObstaculo(Obstaculo(140, 65, 15, e));

    // Particulas: id, x, y, vx, vy, masa, radio
    sim.agregarParticula(Particula(1,  20, 50,  30,  12, 2.0, 3.0));
    sim.agregarParticula(Particula(2, 100, 50, -25,  18, 1.0, 2.5));
    sim.agregarParticula(Particula(3, 180, 80, -20, -22, 3.0, 3.5));
    sim.agregarParticula(Particula(4, 100, 10,  15,  25, 1.5, 2.0));
    sim.agregarParticula(Particula(5,  70, 90, -10, -30, 2.5, 3.0));

    sim.ejecutar(nPasos);

    cout << "Simulacion terminada. Resultados en simulacion.txt" << endl;
    return 0;
}
