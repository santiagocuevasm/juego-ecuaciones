
#pragma once

#include <QWidget>
#include <QElapsedTimer>
#include <QTimer>
#include <QList>
#include <QString>

#include "Nivel.h"
#include "Partida.h"

class QLabel;
class QPushButton;
class QFrame;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QWidget;

class PantallaJuego : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaJuego(QWidget *parent = nullptr);
    void iniciar(int usuarioId);

signals:
    void solicitarMenu();
    void partidaTerminada(const Partida &partida);

private:
    void cargarNivel(int indice);
    void actualizarPantalla();
    void mostrarOpciones();
    void recibirTermino(const QString &termino,
                        bool destinoIzquierdo);
    void comprobarOperacion(const QString &operacion);
    void finalizarNivel(bool superada);
    void abandonarPartida();
    void actualizarTiempo();
    void limpiarOpciones();

    int m_usuarioId = -1;
    int m_indiceNivel = 0;

    QList<Nivel> m_niveles;

    Ecuacion *m_ecuacion = nullptr;
    Partida *m_partida = nullptr;

    QLabel *m_etiquetaLogo = nullptr;
    QLabel *m_etiquetaNivel = nullptr;
    QLabel *m_etiquetaTiempo = nullptr;
    QLabel *m_etiquetaTiempoMovimiento = nullptr;
    QLabel *m_etiquetaPenalizacion = nullptr;
    QLabel *m_etiquetaIgual = nullptr;
    QLabel *m_etiquetaInstruccion = nullptr;

    QFrame *m_zonaIzquierda = nullptr;
    QFrame *m_zonaDerecha = nullptr;

    QWidget *m_contenedorIzquierda = nullptr;
    QWidget *m_contenedorDerecha = nullptr;

    QHBoxLayout *m_disenoIzquierda = nullptr;
    QHBoxLayout *m_disenoDerecha = nullptr;

    QGridLayout *m_opcionesIzquierda = nullptr;
    QGridLayout *m_opcionesDerecha = nullptr;

    QPushButton *m_botonMenu = nullptr;

    QElapsedTimer m_relojNivel;
    QElapsedTimer m_relojMovimiento;

    QTimer *m_temporizador = nullptr;
    QTimer *m_temporizadorAviso = nullptr;

    int m_penalizacionMovimiento = 0;

    bool m_terminoSoltado = false;
    bool m_terminoOrigenIzquierdo = true;
    bool m_ladoDestinoIzquierdo = false;

    QString m_terminoActual;
};

