#include "Nivel.h"

Nivel::Nivel(int numero, int tiempoNivel, int tiempoMovimiento,
             const QStringList &izquierda, const QStringList &derecha,
             const QList<Paso> &pasos)
    : m_numero(numero),
      m_tiempoNivel(tiempoNivel),
      m_tiempoMovimiento(tiempoMovimiento),
      m_izquierda(izquierda),
      m_derecha(derecha),
      m_pasos(pasos)
{
}

int Nivel::numero() const
{
    return m_numero;
}

int Nivel::tiempoNivel() const
{
    return m_tiempoNivel;
}

int Nivel::tiempoMovimiento() const
{
    return m_tiempoMovimiento;
}

Ecuacion Nivel::crearEcuacion() const
{
    return Ecuacion(m_izquierda, m_derecha, m_pasos);
}

QList<Nivel> Nivel::crearTodos()
{
    QList<Nivel> niveles;

    // Nivel 1: 3x + 5 = 20
    niveles.append(Nivel(1, 40, 10, {"3x", "+5"}, {"20"}, {
        {"+5", "Restar 5 en ambos lados",
         {"Sumar 5 en ambos lados", "Dividir entre 5"},
         {"3x"}, {"15"}},

        {"3x", "Dividir entre 3 en ambos lados",
         {"Restar 3 en ambos lados", "Multiplicar por 3"},
         {"x"}, {"5"}}
    }));

    // Nivel 2: 7x - 4 = 2x + 11
    niveles.append(Nivel(2, 35, 9, {"7x", "-4"}, {"2x", "+11"}, {
        {"2x", "Restar 2x en ambos lados",
         {"Sumar 2x en ambos lados", "Dividir entre 2"},
         {"5x", "-4"}, {"11"}},

        {"-4", "Sumar 4 en ambos lados",
         {"Restar 4 en ambos lados", "Multiplicar por 4"},
         {"5x"}, {"15"}},

        {"5x", "Dividir entre 5 en ambos lados",
         {"Restar 5 en ambos lados", "Multiplicar por 5"},
         {"x"}, {"3"}}
    }));

    // Nivel 3: x/4 + 3 = 8
    niveles.append(Nivel(3, 30, 8, {"x/4", "+3"}, {"8"}, {
        {"+3", "Restar 3 en ambos lados",
         {"Sumar 3 en ambos lados", "Multiplicar por 3"},
         {"x/4"}, {"5"}},

        {"x/4", "Multiplicar por 4 en ambos lados",
         {"Dividir entre 4 en ambos lados", "Sumar 4 en ambos lados"},
         {"x"}, {"20"}}
    }));

    // Nivel 4: x² - 5x + 6 = 0
    niveles.append(Nivel(4, 25, 7, {"x²", "-5x", "+6"}, {"0"}, {
        {"+6", "Factorizar el lado izquierdo",
         {"Restar 6 en ambos lados", "Dividir todo entre 6"},
         {"(x-2)(x-3)"}, {"0"}},

        {"(x-2)(x-3)", "Igualar cada factor a 0",
         {"Sumar los dos factores", "Elevar al cuadrado ambos lados"},
         {"x"}, {"2 o 3"}}
    }));

    // Nivel 5: √(2x+1) = 5
    niveles.append(Nivel(5, 20, 6, {"√(2x+1)"}, {"5"}, {
        {"√(2x+1)", "Elevar al cuadrado ambos lados",
         {"Dividir entre 2 en ambos lados", "Restar 1 en ambos lados"},
         {"2x", "+1"}, {"25"}},

        {"+1", "Restar 1 en ambos lados",
         {"Sumar 1 en ambos lados", "Dividir entre 2"},
         {"2x"}, {"24"}},

        {"2x", "Dividir entre 2 en ambos lados",
         {"Restar 2 en ambos lados", "Multiplicar por 2"},
         {"x"}, {"12"}}
    }));

    return niveles;
}


