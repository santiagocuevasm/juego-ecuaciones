
#include "VentanaPrincipal.h"
#include "PantallaLogin.h"
#include "PantallaRegistro.h"
#include "PantallaMenu.h"
#include "PantallaJuego.h"

#include <QMessageBox>

VentanaPrincipal::VentanaPrincipal(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Findx - Juego de Ecuaciones");
    resize(900, 600);

    m_pantallas = new QStackedWidget(this);

    if (!m_gestorBD.abrir()) {
        QMessageBox::critical(
            this,
            "Error",
            "No se pudo abrir la base de datos."
        );
    }

    PantallaLogin *pantallaLogin = new PantallaLogin(this);
    PantallaRegistro *pantallaRegistro = new PantallaRegistro(this);
    PantallaMenu *pantallaMenu = new PantallaMenu(this);
    PantallaJuego *pantallaJuego = new PantallaJuego(this);

    m_pantallas->addWidget(pantallaLogin);
    m_pantallas->addWidget(pantallaRegistro);
    m_pantallas->addWidget(pantallaMenu);
    m_pantallas->addWidget(pantallaJuego);

    setCentralWidget(m_pantallas);

    // Ir al registro.
    connect(pantallaLogin, &PantallaLogin::solicitarRegistro,
            this, [this, pantallaRegistro]() {
        m_pantallas->setCurrentWidget(pantallaRegistro);
    });

    // Volver al inicio de sesión.
    connect(pantallaRegistro, &PantallaRegistro::solicitarLogin,
            this, [this, pantallaLogin]() {
        m_pantallas->setCurrentWidget(pantallaLogin);
    });

    // Registrar usuario.
    connect(pantallaRegistro, &PantallaRegistro::registrarUsuario,
            this,
            [this, pantallaLogin](
                const QString &usuario,
                const QString &nombre,
                const QString &apellido,
                const QString &documento,
                const QDate &fechaNacimiento,
                const QString &clave) {

        if (usuario.isEmpty() || nombre.isEmpty() ||
            apellido.isEmpty() || documento.isEmpty() ||
            clave.isEmpty()) {

            QMessageBox::warning(
                this,
                "Datos incompletos",
                "Debes completar todos los campos."
            );
            return;
        }

        Usuario nuevoUsuario;
        nuevoUsuario.setNombreUsuario(usuario);
        nuevoUsuario.setNombre(nombre);
        nuevoUsuario.setApellido(apellido);
        nuevoUsuario.setDocumento(documento);
        nuevoUsuario.setFechaNacimiento(fechaNacimiento);

        if (m_gestorBD.registrarUsuario(nuevoUsuario, clave)) {
            QMessageBox::information(
                this,
                "Registro exitoso",
                "Tu usuario se ha registrado correctamente."
            );

            m_pantallas->setCurrentWidget(pantallaLogin);
        } else {
            QMessageBox::warning(
                this,
                "Error de registro",
                "No se pudo registrar el usuario. "
                "Es posible que el nombre de usuario ya exista."
            );
        }
    });

    // Iniciar sesión.
    connect(pantallaLogin, &PantallaLogin::iniciarSesion,
            this,
            [this, pantallaMenu](
                const QString &usuario,
                const QString &clave) {

        if (usuario.isEmpty() || clave.isEmpty()) {
            QMessageBox::warning(
                this,
                "Datos incompletos",
                "Escribe tu usuario y contraseña."
            );
            return;
        }

        const int idUsuario =
            m_gestorBD.autenticar(usuario, clave);

        if (idUsuario != -1) {
            m_usuarioActual = idUsuario;

            QMessageBox::information(
                this,
                "Bienvenido",
                "Inicio de sesión exitoso."
            );

            m_pantallas->setCurrentWidget(pantallaMenu);
        } else {
            QMessageBox::warning(
                this,
                "Acceso denegado",
                "El usuario o la contraseña son incorrectos."
            );
        }
    });

    // Cerrar sesión.
    connect(pantallaMenu, &PantallaMenu::solicitarCerrarSesion,
            this, [this, pantallaLogin]() {
        m_usuarioActual = -1;
        m_pantallas->setCurrentWidget(pantallaLogin);
    });

    // Iniciar juego.
    connect(pantallaMenu, &PantallaMenu::solicitarJugar,
            this, [this, pantallaJuego]() {

        if (m_usuarioActual == -1) {
            QMessageBox::warning(
                this,
                "Sesión no válida",
                "Debes iniciar sesión primero."
            );
            return;
        }

        pantallaJuego->iniciar(m_usuarioActual);
        m_pantallas->setCurrentWidget(pantallaJuego);
    });

    // Guardar partida terminada.
    connect(pantallaJuego, &PantallaJuego::partidaTerminada,
            this, [this](const Partida &partida) {

        if (!m_gestorBD.guardarPartida(partida)) {
            QMessageBox::warning(
                this,
                "Error",
                "No se pudo guardar la partida."
            );
        }
    });

    // Volver al menú.
    connect(pantallaJuego, &PantallaJuego::solicitarMenu,
            this, [this, pantallaMenu]() {
        m_pantallas->setCurrentWidget(pantallaMenu);
    });

    // Mostrar estadísticas del usuario.
    connect(pantallaMenu, &PantallaMenu::solicitarProgreso,
            this, [this]() {

        if (m_usuarioActual == -1) {
            QMessageBox::warning(
                this,
                "Mi progreso",
                "Debes iniciar sesión primero."
            );
            return;
        }

        const QString estadisticas =
            m_gestorBD.resumenProgreso(m_usuarioActual);

        QMessageBox::information(
            this,
            "Mi progreso - FindX",
            estadisticas
        );
    });
}

