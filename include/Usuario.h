
#pragma once

#include <QDate>
#include <QString>

/**
 * @brief Datos de un usuario del juego.
 */
class Usuario
{
public:
    Usuario() = default;

    int id() const { return m_id; }

    QString nombreUsuario() const { return m_nombreUsuario; }

    QString nombre() const { return m_nombre; }

    QString apellido() const { return m_apellido; }

    QString documento() const { return m_documento; }

    QDate fechaNacimiento() const { return m_fechaNacimiento; }

    void setId(int valor) { m_id = valor; }

    void setNombreUsuario(const QString &valor)
    {
        m_nombreUsuario = valor;
    }

    void setNombre(const QString &valor)
    {
        m_nombre = valor;
    }

    void setApellido(const QString &valor)
    {
        m_apellido = valor;
    }

    void setDocumento(const QString &valor)
    {
        m_documento = valor;
    }

    void setFechaNacimiento(const QDate &valor)
    {
        m_fechaNacimiento = valor;
    }

    /**
     * @brief Calcula la edad en años cumplidos.
     */
    int calcularEdad() const
    {
        const QDate hoy = QDate::currentDate();

        int edad = hoy.year() - m_fechaNacimiento.year();

        if (hoy < m_fechaNacimiento.addYears(edad)) {
            --edad;
        }

        return edad;
    }

private:
    int m_id = -1;
    QString m_nombreUsuario;
    QString m_nombre;
    QString m_apellido;
    QString m_documento;
    QDate m_fechaNacimiento;
};


