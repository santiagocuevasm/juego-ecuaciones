#pragma once

#include <QWidget>
#include <QDate>
#include <QString>

class QLineEdit;
class QPushButton;
class QDateEdit;

class PantallaRegistro : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaRegistro(QWidget *parent = nullptr);

signals:
    void registrarUsuario(const QString &usuario,
                          const QString &nombre,
                          const QString &apellido,
                          const QString &documento,
                          const QDate &fechaNacimiento,
                          const QString &clave);

    void solicitarLogin();

private:
    QLineEdit *m_usuario;
    QLineEdit *m_nombre;
    QLineEdit *m_apellido;
    QLineEdit *m_documento;
    QLineEdit *m_clave;

    QDateEdit *m_fechaNacimiento;

    QPushButton *m_botonRegistrar;
    QPushButton *m_botonVolver;
};
