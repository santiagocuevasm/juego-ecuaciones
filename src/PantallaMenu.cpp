
#include "PantallaMenu.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

PantallaMenu::PantallaMenu(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(750, 600);

    // Fondo rojo y azul dividido diagonalmente.
    setObjectName("PantallaMenu");
    setAttribute(Qt::WA_StyledBackground, true);

    setStyleSheet(
        "QWidget#PantallaMenu {"
        "background: qlineargradient("
        "x1: 0, y1: 1, x2: 1, y2: 0,"
        "stop: 0.0 #3154AD,"
        "stop: 0.499 #3154AD,"
        "stop: 0.501 #B83232,"
        "stop: 1.0 #B83232"
        ");"
        "}"
    );

    // Logo FindX.
    QLabel *titulo = new QLabel("FindX", this);
    titulo->setAlignment(Qt::AlignCenter);

    titulo->setStyleSheet(
        "font-family: Georgia;"
        "font-size: 76px;"
        "font-weight: bold;"
        "color: black;"
        "background: transparent;"
    );

    // Subtítulo.
    QLabel *subtitulo = new QLabel(
        "el juego de ecuaciones", this
    );

    subtitulo->setAlignment(Qt::AlignCenter);

    subtitulo->setStyleSheet(
        "font-family: Georgia;"
        "font-size: 40px;"
        "font-weight: bold;"
        "color: black;"
        "background: transparent;"
    );

    // Botones del menú.
    m_botonJugar = new QPushButton("Jugar", this);

    m_botonProgreso =
        new QPushButton("Mi progreso", this);

    m_botonCerrarSesion =
        new QPushButton("Cerrar sesión", this);

    // Estilo de los tres botones.
    QString estiloBoton =
        "QPushButton {"
        "font-family: Georgia;"
        "font-size: 42px;"
        "font-weight: bold;"
        "color: black;"
        "background-color: transparent;"
        "border: 5px solid black;"
        "border-radius: 4px;"
        "padding: 8px;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(255,255,255,35);"
        "}";

    m_botonJugar->setStyleSheet(estiloBoton);
    m_botonProgreso->setStyleSheet(estiloBoton);
    m_botonCerrarSesion->setStyleSheet(estiloBoton);

    // Tamaño de los botones.
    m_botonJugar->setFixedSize(400, 90);
    m_botonProgreso->setFixedSize(400, 90);
    m_botonCerrarSesion->setFixedSize(400, 90);

    // Distribución vertical.
    QVBoxLayout *diseno = new QVBoxLayout(this);

    diseno->setContentsMargins(30, 20, 30, 20);
    diseno->setSpacing(18);

    diseno->addStretch();
    diseno->addWidget(titulo);
    diseno->addWidget(subtitulo);
    diseno->addSpacing(20);

    diseno->addWidget(
        m_botonJugar, 0, Qt::AlignHCenter
    );

    diseno->addWidget(
        m_botonProgreso, 0, Qt::AlignHCenter
    );

    diseno->addWidget(
        m_botonCerrarSesion, 0, Qt::AlignHCenter
    );

    diseno->addStretch();

    // Conservar la función de Jugar.
    connect(m_botonJugar, &QPushButton::clicked,
            this, [this]() {
        emit solicitarJugar();
    });

    // Conservar la función de Mi progreso.
    connect(m_botonProgreso, &QPushButton::clicked,
            this, [this]() {
        emit solicitarProgreso();
    });

    // Conservar la función de Cerrar sesión.
    connect(m_botonCerrarSesion, &QPushButton::clicked,
            this, [this]() {
        emit solicitarCerrarSesion();
    });
}

