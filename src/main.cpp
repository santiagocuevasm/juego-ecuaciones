#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("JuegoEcuaciones");

    QLabel etiqueta("Hola, juego de ecuaciones");
    etiqueta.resize(300, 100);
    etiqueta.show();

    return app.exec();
}
