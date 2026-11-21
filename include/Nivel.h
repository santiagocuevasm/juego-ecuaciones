#pragma once
#include "Ecuacion.h"
#include <QList>

class Nivel
{
public:
    Nivel(int numero, int tiempoNivel, int tiempoMovimiento,
          const QStringList &izquierda, const QStringList &derecha,
          const QList<Paso> &pasos);

    int numero() const;
    int tiempoNivel() const;
    int tiempoMovimiento() const;
    Ecuacion crearEcuacion() const;

    static QList<Nivel> crearTodos();

private:
    int m_numero;
    int m_tiempoNivel;
    int m_tiempoMovimiento;
    QStringList m_izquierda;
    QStringList m_derecha;
    QList<Paso> m_pasos;
};
