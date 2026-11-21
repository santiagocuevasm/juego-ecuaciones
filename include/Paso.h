#pragma once
#include <QString>
#include <QStringList>

/** @brief Un movimiento del juego: qué término arrastrar y qué operaciones
se ofrecen. */
struct Paso
{
    QString termino;
    QString operacionCorrecta;
    QStringList operacionesFalsas;
    QStringList izquierda;
    QStringList derecha;
};
