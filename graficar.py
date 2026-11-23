"""
Grafica los resultados de la simulacion de colisiones multiples.

Lee el archivo generado por el programa en C++ (por defecto simulacion.txt)
y dibuja la caja, los obstaculos, la trayectoria de cada particula y los
puntos donde ocurrio cada colision.

Uso:
    python graficar.py [archivo_entrada] [imagen_salida] [--mostrar]

Ejemplo:
    python graficar.py simulacion.txt trayectorias.png --mostrar
"""

import sys
from collections import defaultdict

import matplotlib.pyplot as plt
from matplotlib.lines import Line2D
from matplotlib.patches import Circle, Rectangle


def leer_simulacion(ruta):
    """Lee el archivo y devuelve la caja, los obstaculos, las posiciones y las colisiones."""
    caja = None
    obstaculos = []                      # (x, y, lado, e)
    trayectorias = defaultdict(list)     # id -> [(t, x, y, radio), ...]
    colisiones = []                      # (t, tipo, id1, id2)

    with open(ruta, encoding="utf-8") as f:
        for linea in f:
            partes = linea.split()
            if not partes or partes[0].startswith("#"):
                continue

            tipo = partes[0]
            if tipo == "CAJA":
                caja = (float(partes[1]), float(partes[2]))
            elif tipo == "OBS":
                obstaculos.append(tuple(float(v) for v in partes[2:6]))
            elif tipo == "POS":
                t, pid = float(partes[1]), int(partes[2])
                x, y, r = float(partes[3]), float(partes[4]), float(partes[5])
                trayectorias[pid].append((t, x, y, r))
            elif tipo == "COL":
                colisiones.append((float(partes[1]), partes[2], int(partes[3]), int(partes[4])))

    return caja, obstaculos, trayectorias, colisiones


def posicion_en(trayectoria, t):
    """Posicion (x, y) de la particula en el instante t (o el ultimo registrado antes de t)."""
    ultima = trayectoria[0]
    for punto in trayectoria:
        if punto[0] > t + 1e-9:
            break
        ultima = punto
    return ultima[1], ultima[2]


def graficar(caja, obstaculos, trayectorias, colisiones, salida, mostrar):
    fig, ax = plt.subplots(figsize=(12, 6.5))
    colores = plt.cm.tab10.colors

    # Caja
    ancho, alto = caja
    ax.add_patch(Rectangle((0, 0), ancho, alto, fill=False, linewidth=2.5, edgecolor="black"))

    # Obstaculos
    for k, (x, y, lado, e) in enumerate(obstaculos):
        ax.add_patch(Rectangle((x, y), lado, lado, facecolor="#adb5bd", edgecolor="#495057", linewidth=1.5))
        ax.text(x + lado / 2, y + lado / 2, f"O{k}\ne={e:g}", ha="center", va="center", fontsize=8)

    # Trayectorias
    # id de la particula absorbida -> instante de la fusion
    absorbidas = {id2: t for t, tipo, _, id2 in colisiones if tipo == "PARTICULA"}
    for i, (pid, puntos) in enumerate(sorted(trayectorias.items())):
        color = colores[i % len(colores)]
        xs = [p[1] for p in puntos]
        ys = [p[2] for p in puntos]
        t_fin, x_fin, y_fin, r_fin = puntos[-1]

        # Una particula absorbida deja de aparecer en el archivo despues de la fusion
        absorbida = pid in absorbidas
        etiqueta = f"Particula {pid}" + (f" (absorbida, t = {absorbidas[pid]:g} s)" if absorbida else "")
        ax.plot(xs, ys, color=color, linewidth=1.2, label=etiqueta,
                linestyle="--" if absorbida else "-")

        # Inicio: punto hueco. Fin: circulo con el radio final (solo si sigue activa)
        ax.plot(xs[0], ys[0], "o", markerfacecolor="white", markeredgecolor=color, markersize=7)
        if not absorbida:
            ax.add_patch(Circle((x_fin, y_fin), r_fin, facecolor=color, edgecolor="black", alpha=0.8, zorder=3))

    # Colisiones
    estilos = {
        "PARED":     dict(marker="x", color="#1971c2", s=40),
        "OBSTACULO": dict(marker="^", color="#e8590c", s=50),
        "PARTICULA": dict(marker="*", color="#e03131", s=260, edgecolors="black"),
    }
    for t, tipo, id1, id2 in colisiones:
        x, y = posicion_en(trayectorias[id1], t)
        ax.scatter(x, y, zorder=4, **estilos[tipo])
        if tipo == "PARTICULA":
            ax.annotate(f"{id1} + {id2}\nt = {t:g} s", (x, y), xytext=(10, 10),
                        textcoords="offset points", fontsize=9, fontweight="bold",
                        bbox=dict(boxstyle="round", facecolor="white", alpha=0.85))

    # Leyenda: particulas + tipos de colision
    extras = [
        Line2D([], [], marker="o", linestyle="", markerfacecolor="white", markeredgecolor="gray", label="Inicio"),
        Line2D([], [], marker="o", linestyle="", color="gray", markersize=10, label="Posicion final"),
        Line2D([], [], marker="x", linestyle="", color="#1971c2", label="Colision con pared"),
        Line2D([], [], marker="^", linestyle="", color="#e8590c", label="Colision con obstaculo"),
        Line2D([], [], marker="*", linestyle="", color="#e03131", markersize=14,
               markeredgecolor="black", label="Fusion de particulas"),
    ]
    handles, _ = ax.get_legend_handles_labels()
    ax.legend(handles=handles + extras, loc="upper left", bbox_to_anchor=(1.01, 1), fontsize=9)

    ax.set_xlim(-5, ancho + 5)
    ax.set_ylim(-5, alto + 5)
    ax.set_aspect("equal")
    ax.set_xlabel("Posicion X")
    ax.set_ylabel("Posicion Y")
    ax.set_title("Trayectorias de las particulas y colisiones")
    ax.grid(True, alpha=0.3)
    fig.tight_layout()

    fig.savefig(salida, dpi=150)
    print(f"Grafica guardada en {salida}")
    if mostrar:
        plt.show()


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    entrada = args[0] if len(args) > 0 else "simulacion.txt"
    salida = args[1] if len(args) > 1 else "trayectorias.png"
    mostrar = "--mostrar" in sys.argv

    caja, obstaculos, trayectorias, colisiones = leer_simulacion(entrada)

    conteo = defaultdict(int)
    for _, tipo, _, _ in colisiones:
        conteo[tipo] += 1
    print(f"Particulas: {len(trayectorias)}  |  Colisiones: " +
          ", ".join(f"{tipo} = {n}" for tipo, n in sorted(conteo.items())))

    graficar(caja, obstaculos, trayectorias, colisiones, salida, mostrar)


if __name__ == "__main__":
    main()
