#include <QApplication>
#include "VentanaPrincipal.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QApplication::setApplicationName("JuegoEcuaciones");
    QApplication::setOrganizationName("JuegoEcuaciones");

    VentanaPrincipal ventana;
    ventana.show();

    return app.exec();
}
