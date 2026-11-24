#pragma once

#include <QWidget>
#include <QString>

class QLineEdit;
class QPushButton;

class PantallaLogin : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaLogin(QWidget *parent = nullptr);

signals:
    void iniciarSesion(const QString &usuario,
                       const QString &clave);

    void solicitarRegistro();

private:
    QLineEdit *m_usuario;
    QLineEdit *m_clave;
    QPushButton *m_botonIngresar;
    QPushButton *m_botonRegistro;
};
