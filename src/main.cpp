
#include <QApplication>
#include <QLabel>
#include <QDebug>

#include "Nivel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QApplication::setApplicationName("JuegoEcuaciones");

    // Crear todos los niveles
    QList<Nivel> niveles = Nivel::crearTodos();

    // Seleccionar el nivel 5
    Ecuacion ecuacion = niveles.at(4).crearEcuacion();

    // Mostrar la ecuación inicial
    qDebug() << ecuacion.textoActual();

    // Aplicar las operaciones correctas
    while (!ecuacion.estaResuelta()) {

        ecuacion.aplicarOperacion(
            ecuacion.pasoActual().operacionCorrecta
        );

        qDebug() << ecuacion.textoActual();
    }

    // Mostrar ventana de prueba
    QLabel etiqueta("Hola, juego de ecuaciones");
    etiqueta.resize(300, 100);
    etiqueta.show();

    return app.exec();
}

