#include "Ecuacion.h"
#include <QRandomGenerator>
#include <algorithm>

Ecuacion::Ecuacion(const QStringList &izquierda, const QStringList &derecha,
                   const QList<Paso> &pasos)
    : m_izquierda(izquierda), m_derecha(derecha), m_pasos(pasos)
{
}

QString Ecuacion::textoActual() const
{
    return m_izquierda.join(" ") + " = " + m_derecha.join(" ");
}

QStringList Ecuacion::izquierda() const
{
    return m_izquierda;
}

QStringList Ecuacion::derecha() const
{
    return m_derecha;
}

const Paso &Ecuacion::pasoActual() const
{
    return m_pasos.at(m_indicePaso);
}

QStringList Ecuacion::generarOpciones() const
{
    QStringList opciones = pasoActual().operacionesFalsas;
    opciones.append(pasoActual().operacionCorrecta);

    std::shuffle(opciones.begin(), opciones.end(),
                 *QRandomGenerator::global());

    return opciones;
}

bool Ecuacion::aplicarOperacion(const QString &operacion)
{
    if (operacion != pasoActual().operacionCorrecta) {
        return false;
    }

    m_izquierda = pasoActual().izquierda;
    m_derecha = pasoActual().derecha;
    ++m_indicePaso;

    return true;
}

bool Ecuacion::estaResuelta() const
{
    return m_indicePaso >= m_pasos.size();
}
