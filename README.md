# Simulación de colisiones múltiples

Práctica 5 – Informática II, Universidad de Antioquia. Actividad 2.1.

Simulación en tiempo discreto de *n* partículas circulares dentro de una caja rectangular con 4 obstáculos cuadrados estáticos. Se modelan tres tipos de colisión:

| Colisión | Tipo | Modelo |
|---|---|---|
| Partícula – pared | Perfectamente elástica | Se invierte la componente de la velocidad perpendicular a la pared |
| Partícula – obstáculo | Inelástica (coef. `e`) | `v⊥' = -e·v⊥`, `v∥' = v∥` según el lado del cuadrado impactado |
| Partícula – partícula | Completamente inelástica | Se fusionan: `M = m1 + m2`, `v' = (m1·v1 + m2·v2) / M` |

## Compilación

Proyecto CMake (C++17). Se abre en Qt Creator o se compila con:

```bash
cmake -S . -B build
cmake --build build
```

Al ejecutarlo se genera `simulacion.txt` en el directorio de trabajo.

## Formato del archivo de salida

```
CAJA ancho altura
OBS  k x y lado e
POS  t id x y radio
COL  t tipo id1 id2
```

- `tipo` puede ser `PARED`, `OBSTACULO` o `PARTICULA`.
- `id2` es `-1` para pared, el índice del obstáculo o el id de la otra partícula.

## Diagrama de clases

```mermaid
classDiagram
    direction LR

    class Simulador {
        -Box caja
        -vector~Particula~ particulas
        -vector~Obstaculo~ obstaculos
        -double dt
        -double t
        -ofstream salida
        +Simulador(Box caja, double dt, string archivo)
        +agregarParticula(Particula p)
        +agregarObstaculo(Obstaculo o)
        +ejecutar(int nPasos)
        -paso()
        -detectarColisionesParticulas()
        -registrarEscena()
        -registrarPosiciones()
        -registrarColision(string tipo, int id1, int id2)
    }

    class Box {
        -double ancho
        -double altura
        +Box(double ancho, double altura)
        +rebotarParedes(Particula& p) bool
        +getAncho() double
        +getAltura() double
    }

    class Particula {
        -int id
        -double x
        -double y
        -double vx
        -double vy
        -double masa
        -double radio
        -bool activa
        +Particula(int id, double x, double y, double vx, double vy, double masa, double radio)
        +mover(double dt)
        +colisionaCon(Particula otra) bool
        +fusionar(Particula& otra)
        +getters / setters
    }

    class Obstaculo {
        -double x
        -double y
        -double lado
        -double e
        +Obstaculo(double x, double y, double lado, double e)
        +detectarLado(Particula p) Lado
        +rebotar(Particula& p, Lado l) bool
        +getters
    }

    class Lado {
        <<enumeration>>
        NINGUNO
        IZQUIERDA
        DERECHA
        ARRIBA
        ABAJO
    }

    Simulador "1" *-- "1" Box : contiene
    Simulador "1" *-- "4..*" Particula : contiene
    Simulador "1" *-- "4" Obstaculo : contiene
    Box ..> Particula : usa
    Obstaculo ..> Particula : usa
    Obstaculo ..> Lado : usa
    Particula ..> Particula : se fusiona con
```

## Diagrama de flujo

```mermaid
flowchart TD
    A([Inicio de main]) --> B["Constantes<br/>dt = 0.01, nPasos = 3000, e = 0.7"]

    B --> C["Box(200, 100)<br/>objeto temporal"]
    C --> D["Constructor de Simulador<br/>miembros en orden de declaración:<br/>1. caja ← copia de la Box<br/>2. particulas ← vector vacío<br/>3. obstaculos ← vector vacío<br/>4. dt, t = 0<br/>5. salida ← abre simulacion.txt"]
    D --> D1{"¿Se abrió el archivo?"}
    D1 -- No --> D2["Mensaje de error en cerr"] --> E
    D1 -- Sí --> E

    E["Crear 4 Obstaculo(x, y, lado, e)<br/>agregarObstaculo → push_back (copia)"]
    E --> F["Crear 5 Particula(id, x, y, vx, vy, m, r)<br/>agregarParticula → push_back (copia)"]

    F --> G["sim.ejecutar(nPasos)"]
    G --> H["registrarEscena()<br/>escribe CAJA y OBS"]
    H --> I["registrarPosiciones()<br/>POS en t = 0"]
    I --> J{"¿i menor que nPasos?"}

    J -- Sí --> P0["paso(): t += dt"]
    P0 --> P1{"¿Quedan partículas<br/>por recorrer?"}
    P1 -- Sí --> P2{"¿p activa?"}
    P2 -- No --> P1
    P2 -- Sí --> P3["p.mover(dt)"]
    P3 --> P4{"caja.rebotarParedes(p)"}
    P4 -- true --> P5["registrar COL PARED"] --> P6
    P4 -- false --> P6["Para cada obstáculo k:<br/>lado = detectarLado(p)"]
    P6 --> P7{"lado ≠ NINGUNO y<br/>rebotar(p, lado)"}
    P7 -- true --> P8["registrar COL OBSTACULO"] --> P1
    P7 -- false --> P1

    P1 -- No --> Q1["detectarColisionesParticulas()<br/>para cada par i, j"]
    Q1 --> Q2{"colisionaCon"}
    Q2 -- true --> Q3["registrar COL PARTICULA<br/>i.fusionar(j) → j inactiva"]
    Q2 -- false --> R
    Q3 --> R["registrarPosiciones()<br/>solo partículas activas"]
    R --> J

    J -- No --> S["Mensaje: Simulación terminada"]
    S --> T["Fin de main: se destruye sim<br/>→ ofstream cierra el archivo"]
    T --> U([Fin])
```

## Simplificaciones

- Solo se aplica un rebote si la partícula se mueve hacia la pared u obstáculo, para evitar rebotes dobles en un mismo choque.
- En un choque contra la esquina de un obstáculo se toma el lado con menor penetración.
- Al fusionarse dos partículas, el nuevo cuerpo se ubica en el centro de masa y su radio conserva el área: `r = √(r1² + r2²)`. La partícula absorbida queda inactiva.
