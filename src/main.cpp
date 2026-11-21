
#include <QApplication>
#include <QLabel>
#include <QDebug>
#include <QStandardPaths>

#include "GestorBD.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QApplication::setApplicationName("JuegoEcuaciones");
    QApplication::setOrganizationName("JuegoEcuaciones");

    GestorBD bd;

    // Abrir la base de datos.
    if (!bd.abrir()) {
        qDebug() << "ERROR: No se pudo abrir la base de datos.";
        return 1;
    }

    qDebug() << "Base de datos abierta correctamente.";

    // Crear un usuario de prueba.
    Usuario usuario;
    usuario.setNombreUsuario("usuario_prueba");
    usuario.setNombre("Usuario");
    usuario.setApellido("Prueba");
    usuario.setDocumento("PRUEBA-001");
    usuario.setFechaNacimiento(QDate(2005, 3, 15));

    const QString clave = "ClavePrueba123";

    // Si el usuario todavía no existe, registrarlo.
    int id = bd.autenticar("usuario_prueba", clave);

    if (id == -1) {
        if (!bd.registrarUsuario(usuario, clave)) {
            qDebug() << "ERROR: No se pudo registrar el usuario.";
            return 1;
        }

        qDebug() << "Usuario registrado correctamente.";
        id = bd.autenticar("usuario_prueba", clave);
    } else {
        qDebug() << "El usuario de prueba ya existía.";
    }

    // Comprobar la contraseña correcta.
    qDebug() << "ID con clave correcta:"
             << bd.autenticar("usuario_prueba", clave);

    // Comprobar una contraseña incorrecta.
    qDebug() << "ID con clave incorrecta:"
             << bd.autenticar("usuario_prueba", "ClaveIncorrecta");

    // Mostrar la carpeta donde se guarda la base de datos.
    qDebug() << "Carpeta de la base de datos:"
             << QStandardPaths::writableLocation(
                    QStandardPaths::AppDataLocation);

    // Mostrar una ventana de prueba.
    QLabel etiqueta("Prueba de base de datos SQLite");
    etiqueta.resize(350, 100);
    etiqueta.show();

    return app.exec();
}

