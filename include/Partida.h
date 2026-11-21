
#pragma once

#include "Movimiento.h"
#include <QDateTime>
#include <QList>

/**
 * @brief Una partida de un usuario en un nivel,
 * con todos sus movimientos.
 */
class Partida
{
public:
    Partida(int usuarioId, int nivel)
        : m_usuarioId(usuarioId),
          m_nivel(nivel),
          m_fechaInicio(QDateTime::currentDateTime())
    {
    }

    void registrarMovimiento(int segundosDesdeInicio,
                             const QString &operacion,
                             bool acierto)
    {
        m_movimientos.append(
            Movimiento(
                segundosDesdeInicio / 60,
                segundosDesdeInicio % 60,
                operacion,
                acierto
            )
        );
    }

    void terminar(int tiempoUsado, bool superada)
    {
        m_tiempoUsado = tiempoUsado;
        m_superada = superada;
    }

    int usuarioId() const { return m_usuarioId; }

    int nivel() const { return m_nivel; }

    QDateTime fechaInicio() const { return m_fechaInicio; }

    int tiempoUsado() const { return m_tiempoUsado; }

    bool superada() const { return m_superada; }

    const QList<Movimiento> &movimientos() const
    {
        return m_movimientos;
    }

private:
    int m_usuarioId;
    int m_nivel;
    QDateTime m_fechaInicio;
    int m_tiempoUsado = 0;
    bool m_superada = false;
    QList<Movimiento> m_movimientos;
};


