#pragma once
#include "Paso.h"
#include <QList>
#include <QString>
#include <QStringList>

class Ecuacion
{
public:
    Ecuacion(const QStringList &izquierda,
             const QStringList &derecha,
             const QList<Paso> &pasos);

    QString textoActual() const;
    QStringList izquierda() const;
    QStringList derecha() const;
    const Paso &pasoActual() const;

    QStringList generarOpciones() const;
    bool aplicarOperacion(const QString &operacion);
    bool estaResuelta() const;

private:
    QStringList m_izquierda;
    QStringList m_derecha;
    QList<Paso> m_pasos;
    int m_indicePaso = 0;
};
