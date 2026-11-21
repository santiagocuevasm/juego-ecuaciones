
#pragma once

#include <QString>

/**
 * @brief Un movimiento del jugador: cuándo ocurrió,
 * qué operación eligió y si acertó.
 */
class Movimiento
{
public:
    Movimiento(int minuto, int segundo,
               const QString &operacion, bool acierto)
        : m_minuto(minuto),
          m_segundo(segundo),
          m_operacion(operacion),
          m_acierto(acierto)
    {
    }

    int minuto() const { return m_minuto; }

    int segundo() const { return m_segundo; }

    QString operacion() const { return m_operacion; }

    bool acierto() const { return m_acierto; }

private:
    int m_minuto;
    int m_segundo;
    QString m_operacion;
    bool m_acierto;
};


