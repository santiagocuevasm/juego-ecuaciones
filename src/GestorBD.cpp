
#include "GestorBD.h"
#include "Nivel.h"

#include <QCryptographicHash>
#include <QDebug>
#include <QDir>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QVariant>
#include <QDate>

// Abre la base de datos SQLite.
bool GestorBD::abrir()
{
    const QString carpeta =
        QStandardPaths::writableLocation(
            QStandardPaths::AppDataLocation
        );

    QDir().mkpath(carpeta);

    m_bd = QSqlDatabase::addDatabase("QSQLITE");
    m_bd.setDatabaseName(carpeta + "/juego.db");

    if (!m_bd.open()) {
        qWarning() << m_bd.lastError().text();
        return false;
    }

    QSqlQuery(m_bd).exec("PRAGMA foreign_keys = ON");

    return crearTablas();
}

// Crea las cuatro tablas e inserta los cinco niveles.
bool GestorBD::crearTablas()
{
    const QStringList sentencias = {
        "CREATE TABLE IF NOT EXISTS usuario ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "nombre_usuario TEXT NOT NULL UNIQUE, "
        "hash_contrasena TEXT NOT NULL, "
        "nombre TEXT NOT NULL, "
        "apellido TEXT NOT NULL, "
        "documento TEXT NOT NULL UNIQUE, "
        "fecha_nacimiento TEXT NOT NULL)",

        "CREATE TABLE IF NOT EXISTS nivel ("
        "id INTEGER PRIMARY KEY, "
        "numero INTEGER NOT NULL UNIQUE, "
        "ecuacion TEXT NOT NULL, "
        "tiempo_nivel_s INTEGER NOT NULL, "
        "tiempo_movimiento_s INTEGER NOT NULL)",

        "CREATE TABLE IF NOT EXISTS partida ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "usuario_id INTEGER NOT NULL REFERENCES usuario(id), "
        "nivel_id INTEGER NOT NULL REFERENCES nivel(id), "
        "fecha_inicio TEXT NOT NULL, "
        "tiempo_usado_s INTEGER NOT NULL, "
        "superada INTEGER NOT NULL CHECK (superada IN (0, 1)))",

        "CREATE TABLE IF NOT EXISTS movimiento ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "partida_id INTEGER NOT NULL REFERENCES partida(id), "
        "minuto INTEGER NOT NULL, "
        "segundo INTEGER NOT NULL, "
        "operacion_elegida TEXT NOT NULL, "
        "acierto INTEGER NOT NULL CHECK (acierto IN (0, 1)))"
    };

    QSqlQuery consulta(m_bd);

    for (const QString &sql : sentencias) {
        if (!consulta.exec(sql)) {
            qWarning() << consulta.lastError().text();
            return false;
        }
    }

    for (const Nivel &nivel : Nivel::crearTodos()) {
        consulta.prepare(
            "INSERT OR IGNORE INTO nivel "
            "(id, numero, ecuacion, tiempo_nivel_s, "
            "tiempo_movimiento_s) "
            "VALUES (?, ?, ?, ?, ?)"
        );

        consulta.addBindValue(nivel.numero());
        consulta.addBindValue(nivel.numero());
        consulta.addBindValue(
            nivel.crearEcuacion().textoActual()
        );
        consulta.addBindValue(nivel.tiempoNivel());
        consulta.addBindValue(nivel.tiempoMovimiento());

        if (!consulta.exec()) {
            qWarning() << consulta.lastError().text();
            return false;
        }
    }

    return true;
}

// Calcula el hash SHA-256 de la contraseña.
QString GestorBD::calcularHash(
    const QString &nombreUsuario,
    const QString &clave)
{
    const QByteArray datos =
        (nombreUsuario + ":" + clave).toUtf8();

    return QString::fromLatin1(
        QCryptographicHash::hash(
            datos,
            QCryptographicHash::Sha256
        ).toHex()
    );
}

// Registra un usuario nuevo.
bool GestorBD::registrarUsuario(
    const Usuario &usuario,
    const QString &clave)
{
    QSqlQuery consulta(m_bd);

    consulta.prepare(
        "INSERT INTO usuario "
        "(nombre_usuario, hash_contrasena, nombre, "
        "apellido, documento, fecha_nacimiento) "
        "VALUES (?, ?, ?, ?, ?, ?)"
    );

    consulta.addBindValue(usuario.nombreUsuario());
    consulta.addBindValue(
        calcularHash(usuario.nombreUsuario(), clave)
    );
    consulta.addBindValue(usuario.nombre());
    consulta.addBindValue(usuario.apellido());
    consulta.addBindValue(usuario.documento());
    consulta.addBindValue(
        usuario.fechaNacimiento().toString(Qt::ISODate)
    );

    if (!consulta.exec()) {
        qWarning() << consulta.lastError().text();
        return false;
    }

    return true;
}

// Comprueba el nombre de usuario y la contraseña.
int GestorBD::autenticar(
    const QString &nombreUsuario,
    const QString &clave)
{
    QSqlQuery consulta(m_bd);

    consulta.prepare(
        "SELECT id FROM usuario "
        "WHERE nombre_usuario = ? AND hash_contrasena = ?"
    );

    consulta.addBindValue(nombreUsuario);
    consulta.addBindValue(
        calcularHash(nombreUsuario, clave)
    );

    if (consulta.exec() && consulta.next()) {
        return consulta.value(0).toInt();
    }

    return -1;
}

// Obtiene los datos de un usuario por su identificador.
Usuario GestorBD::obtenerUsuario(int id)
{
    Usuario usuario;

    QSqlQuery consulta(m_bd);

    consulta.prepare(
        "SELECT nombre_usuario, nombre, apellido, "
        "documento, fecha_nacimiento "
        "FROM usuario WHERE id = ?"
    );

    consulta.addBindValue(id);

    if (consulta.exec() && consulta.next()) {
        usuario.setId(id);
        usuario.setNombreUsuario(
            consulta.value(0).toString()
        );
        usuario.setNombre(
            consulta.value(1).toString()
        );
        usuario.setApellido(
            consulta.value(2).toString()
        );
        usuario.setDocumento(
            consulta.value(3).toString()
        );
        usuario.setFechaNacimiento(
            QDate::fromString(
                consulta.value(4).toString(),
                Qt::ISODate
            )
        );
    }

    return usuario;
}

// Actualiza los datos personales de un usuario.
bool GestorBD::actualizarUsuario(const Usuario &usuario)
{
    QSqlQuery consulta(m_bd);

    consulta.prepare(
        "UPDATE usuario SET nombre = ?, apellido = ?, "
        "documento = ?, fecha_nacimiento = ? WHERE id = ?"
    );

    consulta.addBindValue(usuario.nombre());
    consulta.addBindValue(usuario.apellido());
    consulta.addBindValue(usuario.documento());
    consulta.addBindValue(
        usuario.fechaNacimiento().toString(Qt::ISODate)
    );
    consulta.addBindValue(usuario.id());

    if (!consulta.exec()) {
        qWarning() << consulta.lastError().text();
        return false;
    }

    return true;
}

// Guarda una partida y todos sus movimientos.
bool GestorBD::guardarPartida(const Partida &partida)
{
    if (!m_bd.transaction()) {
        qWarning() << m_bd.lastError().text();
        return false;
    }

    QSqlQuery consulta(m_bd);

    consulta.prepare(
        "INSERT INTO partida "
        "(usuario_id, nivel_id, fecha_inicio, "
        "tiempo_usado_s, superada) "
        "VALUES (?, ?, ?, ?, ?)"
    );

    consulta.addBindValue(partida.usuarioId());
    consulta.addBindValue(partida.nivel());
    consulta.addBindValue(
        partida.fechaInicio().toString(Qt::ISODate)
    );
    consulta.addBindValue(partida.tiempoUsado());
    consulta.addBindValue(partida.superada() ? 1 : 0);

    if (!consulta.exec()) {
        qWarning() << consulta.lastError().text();
        m_bd.rollback();
        return false;
    }

    const QVariant idPartida = consulta.lastInsertId();

    for (const Movimiento &movimiento : partida.movimientos()) {
        QSqlQuery consultaMov(m_bd);

        consultaMov.prepare(
            "INSERT INTO movimiento "
            "(partida_id, minuto, segundo, "
            "operacion_elegida, acierto) "
            "VALUES (?, ?, ?, ?, ?)"
        );

        consultaMov.addBindValue(idPartida);
        consultaMov.addBindValue(movimiento.minuto());
        consultaMov.addBindValue(movimiento.segundo());
        consultaMov.addBindValue(movimiento.operacion());
        consultaMov.addBindValue(
            movimiento.acierto() ? 1 : 0
        );

        if (!consultaMov.exec()) {
            qWarning() << consultaMov.lastError().text();
            m_bd.rollback();
            return false;
        }
    }

    if (!m_bd.commit()) {
        qWarning() << m_bd.lastError().text();
        m_bd.rollback();
        return false;
    }

    return true;
}

// Consulta el mejor tiempo de un usuario en un nivel.
int GestorBD::mejorTiempo(int usuarioId, int nivel)
{
    QSqlQuery consulta(m_bd);

    consulta.prepare(
        "SELECT MIN(tiempo_usado_s) FROM partida "
        "WHERE usuario_id = ? AND nivel_id = ? AND superada = 1"
    );

    consulta.addBindValue(usuarioId);
    consulta.addBindValue(nivel);

    if (consulta.exec()
        && consulta.next()
        && !consulta.value(0).isNull()) {
        return consulta.value(0).toInt();
    }

    return -1;
}


QString GestorBD::resumenProgreso(int usuarioId)
{
    QSqlQuery consulta(m_bd);

    consulta.prepare(
        "SELECT COUNT(*), "
        "SUM(CASE WHEN superada = 1 THEN 1 ELSE 0 END), "
        "SUM(CASE WHEN superada = 0 THEN 1 ELSE 0 END) "
        "FROM partida WHERE usuario_id = ?"
    );

    consulta.addBindValue(usuarioId);

    if (!consulta.exec() || !consulta.next()) {
        return "No se pudieron consultar las estadísticas.";
    }

    int total = consulta.value(0).toInt();
    int ganadas = consulta.value(1).toInt();
    int perdidas = consulta.value(2).toInt();

    QSqlQuery movimientos(m_bd);
    movimientos.prepare(
        "SELECT "
        "SUM(CASE WHEN m.acierto = 1 THEN 1 ELSE 0 END), "
        "SUM(CASE WHEN m.acierto = 0 THEN 1 ELSE 0 END) "
        "FROM movimiento m "
        "INNER JOIN partida p ON p.id = m.partida_id "
        "WHERE p.usuario_id = ?"
    );

    movimientos.addBindValue(usuarioId);

    int correctos = 0;
    int incorrectos = 0;

    if (movimientos.exec() && movimientos.next()) {
        correctos = movimientos.value(0).toInt();
        incorrectos = movimientos.value(1).toInt();
    }

    QString texto;
    texto += "RESUMEN DE TU PROGRESO\n\n";
    texto += QString("Partidas jugadas: %1\n").arg(total);
    texto += QString("Partidas superadas: %1\n").arg(ganadas);
    texto += QString("Partidas no superadas: %1\n").arg(perdidas);
    texto += QString("Movimientos correctos: %1\n").arg(correctos);
    texto += QString("Movimientos incorrectos: %1\n\n").arg(incorrectos);

    texto += "MEJOR TIEMPO POR NIVEL\n";

    for (int nivel = 1; nivel <= 5; nivel++) {
        int tiempo = mejorTiempo(usuarioId, nivel);

        QString resultado = tiempo == -1
            ? "Sin completar"
            : QString("%1 min %2 s")
                .arg(tiempo / 60)
                .arg(tiempo % 60);

        texto += QString("Nivel %1: %2\n")
            .arg(nivel)
            .arg(resultado);
    }

    return texto;
}
