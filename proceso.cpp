#include "proceso.h"

std::string Proceso::getOperacion() const
{
    return operacion;
}

void Proceso::setOperacion(const std::string &value)
{
    operacion = value;
}

Proceso &Proceso::operator=(const Proceso &p)
{
    this->id = p.getId();
    this->nombre = p.getNombre();
    this->operacion = p.getOperacion();
    this->tiempoEstimado = p.getTiempoEstimado();
    this->resultadoOperacion = p.getResultadoOperacion();

    return *this;
}

Proceso::Proceso()
{
    
}

std::string Proceso::getNombre() const
{
    return nombre;
}

void Proceso::setNombre(const std::string &value)
{
    nombre = value;
}

int Proceso::getId() const
{
    return id;
}

void Proceso::setId(int value)
{
    id = value;
}

int Proceso::getTiempoEstimado() const
{
    return tiempoEstimado;
}

void Proceso::setTiempoEstimado(int value)
{
    tiempoEstimado = value;
}

int Proceso::getResultadoOperacion() const
{
    return resultadoOperacion;
}

void Proceso::setResultadoOperacion(int value)
{
    resultadoOperacion = value;
}
