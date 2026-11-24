
#pragma once

#include <QMainWindow>
#include <QStackedWidget>

#include "GestorBD.h"

class VentanaPrincipal : public QMainWindow
{
    Q_OBJECT

public:
    explicit VentanaPrincipal(QWidget *parent = nullptr);

private:
    QStackedWidget *m_pantallas;
    GestorBD m_gestorBD;
    int m_usuarioActual = -1;
};

