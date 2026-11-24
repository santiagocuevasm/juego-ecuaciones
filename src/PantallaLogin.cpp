
#include "PantallaLogin.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

PantallaLogin::PantallaLogin(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(750, 650);

    // Fondo diagonal rojo y azul.
    setObjectName("PantallaLogin");
    setAttribute(Qt::WA_StyledBackground, true);

    setStyleSheet(
        "QWidget#PantallaLogin {"
        "background: qlineargradient("
        "x1: 0, y1: 1, x2: 1, y2: 0,"
        "stop: 0.0 #3154AD,"
        "stop: 0.499 #3154AD,"
        "stop: 0.501 #B83232,"
        "stop: 1.0 #B83232"
        ");"
        "}"
    );

    // Contenedor central.
    QWidget *contenido = new QWidget(this);
    contenido->setMaximumWidth(800);
    contenido->setStyleSheet("background: transparent;");

    // Logo FindX.
    QLabel *logo = new QLabel("FindX", contenido);
    logo->setAlignment(Qt::AlignCenter);
    logo->setStyleSheet(
        "font-family: Georgia;"
        "font-size: 76px;"
        "font-weight: bold;"
        "color: black;"
        "background: transparent;"
    );

    // Título.
    QLabel *titulo = new QLabel("Iniciar sesión", contenido);
    titulo->setAlignment(Qt::AlignCenter);
    titulo->setStyleSheet(
        "font-family: Georgia;"
        "font-size: 64px;"
        "font-weight: bold;"
        "color: black;"
        "background: transparent;"
    );

    // Nombre de usuario.
    QLabel *etiquetaUsuario =
        new QLabel("Nombre de usuario:", contenido);

    etiquetaUsuario->setStyleSheet(
        "font-family: Georgia;"
        "font-size: 32px;"
        "color: black;"
        "background: transparent;"
    );

    m_usuario = new QLineEdit(contenido);
    m_usuario->setPlaceholderText("");

    m_usuario->setMinimumHeight(82);
    m_usuario->setStyleSheet(
        "QLineEdit {"
        "font-family: Georgia;"
        "font-size: 25px;"
        "color: black;"
        "background-color: transparent;"
        "border: 5px solid black;"
        "border-radius: 3px;"
        "padding: 8px;"
        "}"
    );

    // Contraseña.
    QLabel *etiquetaClave =
        new QLabel("Contraseña:", contenido);

    etiquetaClave->setStyleSheet(
        "font-family: Georgia;"
        "font-size: 32px;"
        "color: black;"
        "background: transparent;"
    );

    m_clave = new QLineEdit(contenido);
    m_clave->setPlaceholderText("");
    m_clave->setEchoMode(QLineEdit::Password);

    m_clave->setMinimumHeight(82);
    m_clave->setStyleSheet(
        "QLineEdit {"
        "font-family: Georgia;"
        "font-size: 25px;"
        "color: black;"
        "background-color: transparent;"
        "border: 5px solid black;"
        "border-radius: 3px;"
        "padding: 8px;"
        "}"
    );

    // Botones.
    m_botonRegistro =
        new QPushButton("Crear cuenta", contenido);

    m_botonIngresar =
        new QPushButton("Ingresar", contenido);

    QString estiloBoton =
        "QPushButton {"
        "font-family: Georgia;"
        "font-size: 48px;"
        "font-weight: bold;"
        "color: black;"
        "background-color: transparent;"
        "border: 5px solid black;"
        "border-radius: 4px;"
        "padding: 0px 8px;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(255,255,255,35);"
        "}";

    m_botonRegistro->setStyleSheet(estiloBoton);
    m_botonIngresar->setStyleSheet(estiloBoton);

    // Distribución del contenido.
    QVBoxLayout *disenoContenido =
        new QVBoxLayout(contenido);

    disenoContenido->setContentsMargins(0, 0, 0, 0);
    disenoContenido->setSpacing(8);

    disenoContenido->addWidget(logo);
    disenoContenido->addSpacing(15);
    disenoContenido->addWidget(titulo);
    disenoContenido->addSpacing(18);
    disenoContenido->addWidget(etiquetaUsuario);
    disenoContenido->addWidget(m_usuario);
    disenoContenido->addSpacing(2);
    disenoContenido->addWidget(etiquetaClave);
    disenoContenido->addWidget(m_clave);
    disenoContenido->addSpacing(10);
    disenoContenido->addWidget(
        m_botonRegistro, 0, Qt::AlignHCenter
    );
    disenoContenido->addSpacing(15);
    disenoContenido->addWidget(
        m_botonIngresar, 0, Qt::AlignHCenter
    );

    // Centrar el contenido.
    QHBoxLayout *fila = new QHBoxLayout;
    fila->addStretch();
    fila->addWidget(contenido);
    fila->addStretch();

    QVBoxLayout *disenoPrincipal =
        new QVBoxLayout(this);

    disenoPrincipal->setContentsMargins(30, 0, 30, 0);
    disenoPrincipal->addStretch();
    disenoPrincipal->addLayout(fila);
    disenoPrincipal->addStretch();

    // Conservar el funcionamiento del inicio de sesión.
    connect(m_botonIngresar, &QPushButton::clicked,
            this, [this]() {
        emit iniciarSesion(
            m_usuario->text(),
            m_clave->text()
        );
    });

    // Conservar el acceso al registro.
    connect(m_botonRegistro, &QPushButton::clicked,
            this, [this]() {
        emit solicitarRegistro();
    });
}

