
#pragma once

#include "Partida.h"
#include "Usuario.h"

#include <QSqlDatabase>
#include <QString>

class GestorBD
{
public:
    bool abrir();

    bool registrarUsuario(const Usuario &usuario,
                          const QString &clave);

    bool actualizarUsuario(const Usuario &usuario);

    Usuario obtenerUsuario(int id);

    int autenticar(const QString &nombreUsuario,
                   const QString &clave);

    bool guardarPartida(const Partida &partida);

    int mejorTiempo(int usuarioId, int nivel);

    QString resumenProgreso(int usuarioId);

private:
    bool crearTablas();

    static QString calcularHash(const QString &nombreUsuario,
                                const QString &clave);

    QSqlDatabase m_bd;
};

