
#pragma once

#include <QWidget>

class QPushButton;

class PantallaMenu : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaMenu(QWidget *parent = nullptr);

signals:
    void solicitarJugar();
    void solicitarProgreso();
    void solicitarCerrarSesion();

private:
    QPushButton *m_botonJugar;
    QPushButton *m_botonProgreso;
    QPushButton *m_botonCerrarSesion;
};

