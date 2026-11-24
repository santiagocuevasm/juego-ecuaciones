
#include "PantallaRegistro.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDateEdit>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QFont>

PantallaRegistro::PantallaRegistro(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet(R"(
        QWidget {
            background: qlineargradient(
                x1: 0, y1: 0,
                x2: 1, y2: 1,
                stop: 0 #b83232,
                stop: 0.49 #b83232,
                stop: 0.50 #2f51ad,
                stop: 1 #2f51ad
            );
            color: black;
            font-family: Georgia;
        }

        QLabel {
            background: transparent;
            color: black;
            font-weight: bold;
        }

        QLineEdit, QDateEdit {
            background: transparent;
            color: black;
            border: 4px solid black;
            border-radius: 3px;
            padding: 5px;
            font-size: 19px;
            min-height: 38px;
        }

        QLineEdit:focus, QDateEdit:focus {
            border: 5px solid black;
        }

        QPushButton {
            background: transparent;
            color: black;
            border: 4px solid black;
            border-radius: 3px;
            padding: 5px 12px;
            font-size: 22px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: rgba(255, 255, 255, 45);
        }

        QPushButton:pressed {
            background-color: rgba(0, 0, 0, 35);
        }
    )");

    // Título principal.
    QLabel *logo = new QLabel("FindX", this);
    logo->setAlignment(Qt::AlignCenter);
    logo->setStyleSheet(
        "font-size: 52px;"
        "font-weight: bold;"
        "background: transparent;"
    );

    // Subtítulo.
    QLabel *titulo = new QLabel("Crear cuenta", this);
    titulo->setAlignment(Qt::AlignCenter);
    titulo->setStyleSheet(
        "font-size: 34px;"
        "font-weight: bold;"
        "background: transparent;"
    );

    // Campos del formulario.
    m_usuario = new QLineEdit(this);
    m_usuario->setPlaceholderText("Nombre de usuario");

    m_nombre = new QLineEdit(this);
    m_nombre->setPlaceholderText("Nombre");

    m_apellido = new QLineEdit(this);
    m_apellido->setPlaceholderText("Apellido");

    m_documento = new QLineEdit(this);
    m_documento->setPlaceholderText("Documento");

    m_fechaNacimiento = new QDateEdit(this);
    m_fechaNacimiento->setCalendarPopup(true);
    m_fechaNacimiento->setDisplayFormat("dd/MM/yyyy");

    m_clave = new QLineEdit(this);
    m_clave->setPlaceholderText("Contraseña");
    m_clave->setEchoMode(QLineEdit::Password);

    // Formulario.
    QFormLayout *formulario = new QFormLayout;

    formulario->addRow("Usuario:", m_usuario);
    formulario->addRow("Nombre:", m_nombre);
    formulario->addRow("Apellido:", m_apellido);
    formulario->addRow("Documento:", m_documento);
    formulario->addRow("Fecha de nacimiento:", m_fechaNacimiento);
    formulario->addRow("Contraseña:", m_clave);

    formulario->setSpacing(9);
    formulario->setLabelAlignment(
        Qt::AlignRight | Qt::AlignVCenter
    );

    // Botones.
    m_botonRegistrar = new QPushButton("Crear cuenta", this);
    m_botonVolver = new QPushButton(
        "Volver al inicio de sesión", this
    );

    m_botonRegistrar->setMinimumHeight(48);
    m_botonVolver->setMinimumHeight(48);

    // Diseño general.
    QVBoxLayout *diseno = new QVBoxLayout(this);

    diseno->addWidget(logo);
    diseno->addWidget(titulo);
    diseno->addSpacing(10);
    diseno->addLayout(formulario);
    diseno->addSpacing(12);
    diseno->addWidget(m_botonRegistrar);
    diseno->addWidget(m_botonVolver);

    diseno->setContentsMargins(100, 12, 100, 18);
    diseno->setSpacing(8);

    // Registrar usuario.
    connect(m_botonRegistrar, &QPushButton::clicked,
            this, [this]() {

        emit registrarUsuario(
            m_usuario->text(),
            m_nombre->text(),
            m_apellido->text(),
            m_documento->text(),
            m_fechaNacimiento->date(),
            m_clave->text()
        );
    });

    // Volver al inicio de sesión.
    connect(m_botonVolver, &QPushButton::clicked,
            this, [this]() {

        emit solicitarLogin();
    });
}

