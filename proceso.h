#ifndef PROCESO_H
#define PROCESO_H

#include <iostream>

class Proceso
{
private:
    std::string nombre;
    std::string operacion;
    int id;
    int tiempoEstimado;
    int resultadoOperacion;

public:
    Proceso();
    std::string getNombre() const;
    void setNombre(const std::string &value);
    int getId() const;
    void setId(int value);
    int getTiempoEstimado() const;
    void setTiempoEstimado(int value);
    int getResultadoOperacion() const;
    void setResultadoOperacion(int value);
    std::string getOperacion() const;
    void setOperacion(const std::string &value);

    //Operators
    Proceso &operator=(const Proceso& p);
};

#endif // PROCESO_H
