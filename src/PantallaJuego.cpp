
#include "PantallaJuego.h"

#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QMessageBox>
#include <QLayoutItem>
#include <QRandomGenerator>
#include <QPointer>
#include <QTimer>

#include <algorithm>
#include <functional>

// Etiqueta que permite arrastrar términos.
class EtiquetaArrastrable : public QLabel
{
public:
    explicit EtiquetaArrastrable(
        const QString &texto,
        QWidget *parent = nullptr
    ) : QLabel(texto, parent)
    {
        setAlignment(Qt::AlignCenter);
        setMinimumSize(65, 55);
        setCursor(Qt::OpenHandCursor);

        setStyleSheet(
            "QLabel {"
            "background-color: rgba(255,255,255,0.92);"
            "color: black;"
            "border: 3px solid black;"
            "border-radius: 0px;"
            "font-family: Georgia;"
            "font-size: 25px;"
            "font-weight: bold;"
            "padding: 6px;"
            "}"
        );
    }

    void establecerCallbackFinArrastre(
        std::function<void()> callback)
    {
        m_callbackFinArrastre = callback;
    }

protected:
    void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton) {
            m_posicionInicial = event->position().toPoint();
            setCursor(Qt::ClosedHandCursor);
        }

        QLabel::mousePressEvent(event);
    }

    void mouseMoveEvent(QMouseEvent *event) override
    {
        if (!(event->buttons() & Qt::LeftButton)) {
            return;
        }

        const QPoint desplazamiento =
            event->position().toPoint() - m_posicionInicial;

        if (desplazamiento.manhattanLength() <
            QApplication::startDragDistance()) {
            return;
        }

        QDrag *arrastre = new QDrag(this);
        QMimeData *datos = new QMimeData;

        datos->setText(text());
        arrastre->setMimeData(datos);

        arrastre->exec(Qt::MoveAction);

        setCursor(Qt::OpenHandCursor);

        if (m_callbackFinArrastre) {
            auto callback = m_callbackFinArrastre;

            QTimer::singleShot(0, qApp, [callback]() {
                callback();
            });
        }
    }

private:
    QPoint m_posicionInicial;
    std::function<void()> m_callbackFinArrastre;
};


// Mitad de la pantalla que recibe los términos.
class ZonaDrop : public QFrame
{
public:
    explicit ZonaDrop(
        bool ladoIzquierdo,
        QWidget *parent = nullptr
    ) : QFrame(parent),
        m_ladoIzquierdo(ladoIzquierdo)
    {
        setAcceptDrops(true);
    }

    void establecerCallback(
        std::function<void(const QString &, bool)> callback)
    {
        m_callback = callback;
    }

protected:
    void dragEnterEvent(QDragEnterEvent *event) override
    {
        if (event->mimeData()->hasText()) {
            event->acceptProposedAction();
        }
    }

    void dropEvent(QDropEvent *event) override
    {
        if (event->mimeData()->hasText()) {
            if (m_callback) {
                m_callback(
                    event->mimeData()->text(),
                    m_ladoIzquierdo
                );
            }

            event->acceptProposedAction();
        }
    }

private:
    bool m_ladoIzquierdo;
    std::function<void(const QString &, bool)> m_callback;
};


// Limpia un diseño y elimina sus widgets.
static void limpiarDiseno(QLayout *diseno)
{
    while (QLayoutItem *item = diseno->takeAt(0)) {
        if (QWidget *widget = item->widget()) {
            delete widget;
        }

        if (QLayout *hijo = item->layout()) {
            limpiarDiseno(hijo);
            delete hijo;
        }

        delete item;
    }
}


// Constructor de la pantalla.
PantallaJuego::PantallaJuego(QWidget *parent)
    : QWidget(parent)
{
    m_niveles = Nivel::crearTodos();

    setMinimumSize(750, 500);

    setStyleSheet(
        "PantallaJuego {"
        "background-color: #B83232;"
        "}"
    );

    // Encabezado.
    m_etiquetaLogo = new QLabel("FindX", this);
    m_etiquetaNivel = new QLabel(this);
    m_etiquetaTiempo = new QLabel(this);
    m_etiquetaTiempoMovimiento = new QLabel(this);
    m_etiquetaPenalizacion = new QLabel(this);

    m_etiquetaLogo->setStyleSheet(
        "font-family: Georgia;"
        "font-size: 38px;"
        "font-weight: bold;"
        "color: black;"
        "background: transparent;"
    );

    m_etiquetaNivel->setStyleSheet(
        "font-family: Georgia;"
        "font-size: 17px;"
        "font-weight: bold;"
        "color: black;"
        "background: transparent;"
    );

    m_etiquetaTiempo->setStyleSheet(
        "font-family: Georgia;"
        "font-size: 16px;"
        "font-weight: bold;"
        "color: black;"
        "background: transparent;"
    );

    m_etiquetaTiempoMovimiento->setStyleSheet(
        "font-family: Georgia;"
        "font-size: 16px;"
        "font-weight: bold;"
        "color: black;"
        "background: transparent;"
    );

    m_etiquetaPenalizacion->setStyleSheet(
        "font-family: Georgia;"
        "font-size: 16px;"
        "font-weight: bold;"
        "color: black;"
        "background: transparent;"
    );

    m_etiquetaPenalizacion->hide();

    auto *encabezado = new QWidget(this);
    encabezado->setStyleSheet(
        "background-color: #B83232;"
        "border-bottom: 3px solid black;"
    );

    auto *disenoEncabezado = new QHBoxLayout(encabezado);
    disenoEncabezado->setContentsMargins(18, 8, 18, 8);
    disenoEncabezado->addWidget(m_etiquetaLogo);
    disenoEncabezado->addStretch();
    disenoEncabezado->addWidget(m_etiquetaNivel);
    disenoEncabezado->addSpacing(15);
    disenoEncabezado->addWidget(m_etiquetaTiempo);
    disenoEncabezado->addSpacing(15);
    disenoEncabezado->addWidget(m_etiquetaTiempoMovimiento);
    disenoEncabezado->addWidget(m_etiquetaPenalizacion);

    // Mitades roja y azul.
    auto *zonaIzquierda = new ZonaDrop(true, this);
    auto *zonaDerecha = new ZonaDrop(false, this);

    m_zonaIzquierda = zonaIzquierda;
    m_zonaDerecha = zonaDerecha;

    zonaIzquierda->setStyleSheet(
        "QFrame {"
        "background-color: #B83232;"
        "border: 2px solid black;"
        "}"
    );

    zonaDerecha->setStyleSheet(
        "QFrame {"
        "background-color: #3154AD;"
        "border: 2px solid black;"
        "}"
    );

    // Distribución de la mitad izquierda.
    auto *disenoZonaIzquierda =
        new QVBoxLayout(zonaIzquierda);

    m_disenoIzquierda = new QHBoxLayout;
    m_contenedorIzquierda = new QWidget(zonaIzquierda);
    m_opcionesIzquierda =
        new QGridLayout(m_contenedorIzquierda);

    m_opcionesIzquierda->setSpacing(15);
    m_opcionesIzquierda->setContentsMargins(15, 15, 15, 15);

    disenoZonaIzquierda->addLayout(m_disenoIzquierda);
    disenoZonaIzquierda->addWidget(
        m_contenedorIzquierda, 1
    );

    // Distribución de la mitad derecha.
    auto *disenoZonaDerecha =
        new QVBoxLayout(zonaDerecha);

    m_disenoDerecha = new QHBoxLayout;
    m_contenedorDerecha = new QWidget(zonaDerecha);
    m_opcionesDerecha =
        new QGridLayout(m_contenedorDerecha);

    m_opcionesDerecha->setSpacing(15);
    m_opcionesDerecha->setContentsMargins(15, 15, 15, 15);

    disenoZonaDerecha->addLayout(m_disenoDerecha);
    disenoZonaDerecha->addWidget(
        m_contenedorDerecha, 1
    );

    // Símbolo igual en el centro.
    m_etiquetaIgual = new QLabel("=", this);
    m_etiquetaIgual->setAlignment(Qt::AlignCenter);
    m_etiquetaIgual->setFixedWidth(45);

    m_etiquetaIgual->setStyleSheet(
        "font-family: Georgia;"
        "font-size: 42px;"
        "font-weight: bold;"
        "color: black;"
        "background-color: #E5E5E5;"
        "border: 3px solid black;"
    );

    // Fila central.
    auto *filaJuego = new QHBoxLayout;
    filaJuego->setSpacing(0);
    filaJuego->addWidget(zonaIzquierda, 1);
    filaJuego->addWidget(m_etiquetaIgual);
    filaJuego->addWidget(zonaDerecha, 1);

    // Instrucción inferior.
    m_etiquetaInstruccion = new QLabel(this);
    m_etiquetaInstruccion->setAlignment(Qt::AlignCenter);
    m_etiquetaInstruccion->setWordWrap(true);
    m_etiquetaInstruccion->setMinimumHeight(44);
    m_etiquetaInstruccion->setMaximumHeight(58);

    m_etiquetaInstruccion->setStyleSheet(
        "font-family: Georgia;"
        "font-size: 15px;"
        "font-weight: bold;"
        "color: black;"
        "background-color: #E5E5E5;"
        "border: 3px solid black;"
        "padding: 8px;"
    );

    m_botonMenu = new QPushButton(
        "Volver al menú", this
    );

    m_botonMenu->setMinimumHeight(42);
    m_botonMenu->setCursor(Qt::PointingHandCursor);

    m_botonMenu->setStyleSheet(
        "QPushButton {"
        "font-family: Georgia;"
        "font-size: 15px;"
        "font-weight: bold;"
        "color: black;"
        "background-color: #E5E5E5;"
        "border: 3px solid black;"
        "padding: 7px;"
        "}"
        "QPushButton:hover {"
        "background-color: white;"
        "}"
    );

    // Diseño principal.
    auto *disenoPrincipal = new QVBoxLayout(this);
    disenoPrincipal->setContentsMargins(0, 0, 0, 0);
    disenoPrincipal->setSpacing(0);

    disenoPrincipal->addWidget(encabezado);
    disenoPrincipal->addLayout(filaJuego, 1);
    disenoPrincipal->addWidget(m_etiquetaInstruccion);
    disenoPrincipal->addWidget(m_botonMenu);

    // Temporizadores.
    m_temporizador = new QTimer(this);

    connect(
        m_temporizador,
        &QTimer::timeout,
        this,
        &PantallaJuego::actualizarTiempo
    );

    m_temporizadorAviso = new QTimer(this);
    m_temporizadorAviso->setSingleShot(true);

    connect(
        m_temporizadorAviso,
        &QTimer::timeout,
        m_etiquetaPenalizacion,
        &QLabel::hide
    );

    connect(
        m_botonMenu,
        &QPushButton::clicked,
        this,
        &PantallaJuego::abandonarPartida
    );

    auto callback = [this](
        const QString &termino,
        bool destinoIzquierdo
    ) {
        recibirTermino(termino, destinoIzquierdo);
    };

    zonaIzquierda->establecerCallback(callback);
    zonaDerecha->establecerCallback(callback);
}


// Inicia una partida.
void PantallaJuego::iniciar(int usuarioId)
{
    m_temporizador->stop();
    m_temporizadorAviso->stop();

    delete m_ecuacion;
    m_ecuacion = nullptr;

    delete m_partida;
    m_partida = nullptr;

    m_usuarioId = usuarioId;
    m_indiceNivel = 0;

    cargarNivel(m_indiceNivel);
}


// Carga un nivel.
void PantallaJuego::cargarNivel(int indice)
{
    m_temporizador->stop();

    if (indice >= m_niveles.size()) {
        delete m_ecuacion;
        m_ecuacion = nullptr;

        QMessageBox::information(
            this,
            "¡Felicitaciones!",
            "¡Completaste todos los niveles!"
        );

        emit solicitarMenu();
        return;
    }

    delete m_ecuacion;
    m_ecuacion = nullptr;

    delete m_partida;
    m_partida = nullptr;

    m_indiceNivel = indice;

    const Nivel &nivel = m_niveles.at(indice);

    m_ecuacion = new Ecuacion(nivel.crearEcuacion());
    m_partida = new Partida(m_usuarioId, nivel.numero());

    m_penalizacionMovimiento = 0;
    m_terminoSoltado = false;
    m_terminoActual.clear();

    m_etiquetaNivel->setText(
        "Nivel " + QString::number(nivel.numero())
    );

    m_etiquetaPenalizacion->hide();

    m_etiquetaInstruccion->setText(
        "Arrastra el término al otro lado."
    );

    limpiarOpciones();

    m_relojNivel.start();
    m_relojMovimiento.start();

    m_temporizador->start(1000);

    actualizarPantalla();
    actualizarTiempo();
}


// Actualiza los términos de la ecuación.
void PantallaJuego::actualizarPantalla()
{
    if (m_ecuacion == nullptr ||
        m_ecuacion->estaResuelta()) {
        return;
    }

    const Paso &paso = m_ecuacion->pasoActual();

    QStringList izquierda = m_ecuacion->izquierda();
    QStringList derecha = m_ecuacion->derecha();

    if (m_terminoSoltado) {
        if (m_terminoOrigenIzquierdo) {
            izquierda.removeOne(m_terminoActual);
        } else {
            derecha.removeOne(m_terminoActual);
        }

        if (m_ladoDestinoIzquierdo) {
            izquierda.append(m_terminoActual);
        } else {
            derecha.append(m_terminoActual);
        }
    }

    limpiarDiseno(m_disenoIzquierda);
    limpiarDiseno(m_disenoDerecha);

    auto agregarTerminos = [this, &paso](
        QHBoxLayout *diseno,
        const QStringList &terminos
    ) {
        for (const QString &termino : terminos) {
            if (!m_terminoSoltado &&
                termino == paso.termino) {

                auto *etiqueta = new EtiquetaArrastrable(
                    termino,
                    diseno->parentWidget()
                );

                QPointer<PantallaJuego> pantallaSegura(this);

                etiqueta->establecerCallbackFinArrastre(
                    [pantallaSegura]() {
                        if (!pantallaSegura) {
                            return;
                        }

                        if (pantallaSegura->m_terminoSoltado &&
                            pantallaSegura->m_ecuacion != nullptr &&
                            pantallaSegura->m_partida != nullptr) {

                            pantallaSegura->actualizarPantalla();
                            pantallaSegura->mostrarOpciones();
                        }
                    }
                );

                diseno->addWidget(etiqueta);
            } else {
                auto *etiqueta = new QLabel(
                    termino,
                    diseno->parentWidget()
                );

                etiqueta->setAlignment(Qt::AlignCenter);
                etiqueta->setMinimumHeight(55);

                etiqueta->setStyleSheet(
                    "font-family: Georgia;"
                    "font-size: 27px;"
                    "font-weight: bold;"
                    "color: black;"
                    "background-color: transparent;"
                    "padding: 5px;"
                );

                diseno->addWidget(etiqueta);
            }
        }

        diseno->addStretch();
    };

    agregarTerminos(m_disenoIzquierda, izquierda);
    agregarTerminos(m_disenoDerecha, derecha);

    if (m_terminoSoltado) {
        m_etiquetaInstruccion->setText(
            "Elige una operación."
        );
    } else {
        m_etiquetaInstruccion->setText(
            "Arrastra " + paso.termino + " al otro lado."
        );
    }
}


// Recibe el término arrastrado.
void PantallaJuego::recibirTermino(
    const QString &termino,
    bool destinoIzquierdo
)
{
    if (m_partida == nullptr ||
        m_ecuacion == nullptr ||
        m_terminoSoltado) {
        return;
    }

    const Paso &paso = m_ecuacion->pasoActual();

    if (termino != paso.termino) {
        return;
    }

    const QStringList izquierda = m_ecuacion->izquierda();
    const QStringList derecha = m_ecuacion->derecha();

    const bool origenIzquierdo =
        izquierda.contains(termino);

    const bool origenDerecho =
        derecha.contains(termino);

    if (!origenIzquierdo && !origenDerecho) {
        return;
    }

    const bool destinoCorrecto =
        origenIzquierdo ? !destinoIzquierdo
                        : destinoIzquierdo;

    if (!destinoCorrecto) {
        m_etiquetaInstruccion->setText(
            "Suéltalo en el lado contrario."
        );
        return;
    }

    m_terminoActual = termino;
    m_terminoOrigenIzquierdo = origenIzquierdo;
    m_ladoDestinoIzquierdo = destinoIzquierdo;
    m_terminoSoltado = true;
}


// Limpia las opciones de ambas mitades.
void PantallaJuego::limpiarOpciones()
{
    limpiarDiseno(m_opcionesIzquierda);
    limpiarDiseno(m_opcionesDerecha);
}


// Muestra las opciones en posiciones aleatorias.
void PantallaJuego::mostrarOpciones()
{
    if (m_ecuacion == nullptr ||
        !m_terminoSoltado) {
        return;
    }

    limpiarOpciones();

    const QStringList opciones =
        m_ecuacion->generarOpciones();

    QList<int> posiciones;

    for (int i = 0; i < 12; ++i) {
        posiciones.append(i);
    }

    std::shuffle(
        posiciones.begin(),
        posiciones.end(),
        *QRandomGenerator::global()
    );

    QGridLayout *destino =
        m_ladoDestinoIzquierdo
        ? m_opcionesIzquierda
        : m_opcionesDerecha;

    for (int i = 0; i < opciones.size(); ++i) {
        const QString opcion = opciones.at(i);

        auto *boton = new QPushButton(opcion, this);

        boton->setMinimumSize(85, 55);
        boton->setCursor(Qt::PointingHandCursor);

        boton->setStyleSheet(
            "QPushButton {"
            "font-family: Georgia;"
            "background-color: #F5F5F5;"
            "color: black;"
            "border: 3px solid black;"
            "border-radius: 0px;"
            "font-size: 20px;"
            "font-weight: bold;"
            "padding: 8px;"
            "}"
            "QPushButton:hover {"
            "background-color: white;"
            "}"
        );

        const int posicion = posiciones.at(i);
        const int fila = posicion / 3;
        const int columna = posicion % 3;

        destino->addWidget(boton, fila, columna);

        connect(
            boton,
            &QPushButton::clicked,
            this,
            [this, opcion]() {
                comprobarOperacion(opcion);
            }
        );
    }
}


// Comprueba la operación seleccionada.
void PantallaJuego::comprobarOperacion(
    const QString &operacion
)
{
    if (!m_terminoSoltado ||
        m_ecuacion == nullptr ||
        m_partida == nullptr) {
        return;
    }

    const int segundos =
        static_cast<int>(m_relojNivel.elapsed() / 1000);

    const bool correcta =
        m_ecuacion->aplicarOperacion(operacion);

    m_partida->registrarMovimiento(
        segundos,
        operacion,
        correcta
    );

    if (!correcta) {
        m_penalizacionMovimiento += 3;

        m_etiquetaPenalizacion->setText("−3 s");
        m_etiquetaPenalizacion->show();
        m_temporizadorAviso->start(1000);

        m_etiquetaInstruccion->setText(
            "Incorrecto: −3 s. Intenta otra vez."
        );

        actualizarTiempo();
        return;
    }

    m_penalizacionMovimiento = 0;
    m_relojMovimiento.restart();

    m_terminoSoltado = false;
    m_terminoActual.clear();

    limpiarOpciones();

    if (m_ecuacion->estaResuelta()) {
        finalizarNivel(true);
        return;
    }

    actualizarPantalla();
    actualizarTiempo();
}


// Finaliza el nivel.
void PantallaJuego::finalizarNivel(bool superada)
{
    m_temporizador->stop();
    m_temporizadorAviso->stop();
    m_etiquetaPenalizacion->hide();

    if (m_partida == nullptr) {
        return;
    }

    const Nivel &nivel = m_niveles.at(m_indiceNivel);

    const int tiempo =
        static_cast<int>(m_relojNivel.elapsed() / 1000);

    m_partida->terminar(tiempo, superada);

    emit partidaTerminada(*m_partida);

    delete m_partida;
    m_partida = nullptr;

    if (!superada) {
        QMessageBox::warning(
            this,
            "Tiempo agotado",
            "No completaste el nivel " +
            QString::number(nivel.numero()) +
            ". Tu resultado se guardó."
        );

        emit solicitarMenu();
        return;
    }

    QMessageBox::information(
        this,
        "Nivel completado",
        "¡Excelente! Superaste el nivel " +
        QString::number(nivel.numero()) +
        ".\nTiempo: " + QString::number(tiempo) +
        " segundos."
    );

    ++m_indiceNivel;
    cargarNivel(m_indiceNivel);
}


// Abandona la partida.
void PantallaJuego::abandonarPartida()
{
    m_temporizador->stop();
    m_temporizadorAviso->stop();
    m_etiquetaPenalizacion->hide();

    if (m_partida != nullptr) {
        const int tiempo =
            static_cast<int>(m_relojNivel.elapsed() / 1000);

        m_partida->terminar(tiempo, false);
        emit partidaTerminada(*m_partida);

        delete m_partida;
        m_partida = nullptr;
    }

    emit solicitarMenu();
}


// Actualiza los cronómetros.
void PantallaJuego::actualizarTiempo()
{
    if (m_indiceNivel < 0 ||
        m_indiceNivel >= m_niveles.size() ||
        m_partida == nullptr) {
        return;
    }

    const Nivel &nivel = m_niveles.at(m_indiceNivel);

    const int tiempoNivel =
        static_cast<int>(m_relojNivel.elapsed() / 1000);

    const int tiempoMovimiento =
        static_cast<int>(m_relojMovimiento.elapsed() / 1000);

    const int restanteNivel =
        nivel.tiempoNivel() - tiempoNivel;

    const int restanteMovimiento =
        nivel.tiempoMovimiento() -
        tiempoMovimiento -
        m_penalizacionMovimiento;

    m_etiquetaTiempo->setText(
        "Tiempo: " +
        QString::number(qMax(0, restanteNivel)) +
        " s"
    );

    m_etiquetaTiempoMovimiento->setText(
        "Movimiento: " +
        QString::number(qMax(0, restanteMovimiento)) +
        " s"
    );

    if (restanteNivel <= 0 || restanteMovimiento <= 0) {
        finalizarNivel(false);
    }
}

