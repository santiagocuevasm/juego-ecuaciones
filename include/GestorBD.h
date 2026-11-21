
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

    // Devuelve el ID del usuario o -1 si los datos son incorrectos.
    int autenticar(const QString &nombreUsuario,
                   const QString &clave);

    bool guardarPartida(const Partida &partida);

    // Devuelve -1 si no existen partidas superadas.
    int mejorTiempo(int usuarioId, int nivel);

private:
    bool crearTablas();

    static QString calcularHash(const QString &nombreUsuario,
                                const QString &clave);

    QSqlDatabase m_bd;
};

