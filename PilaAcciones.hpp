#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "Accion.hpp"

// ====================================================================
// PRACTICA EVALUADA 03 - MODELO B (PILA ENLAZADA)
// Integrantes:
//   1. Mathias Molina - C.I. V-32.598.208 - Rol: header y desarrollador en la parte C
//   2. Miguel Andrade - C.I. V-31.905.358 - Rol: desarrollador en la parte A
//   3. Salomé Rondón - C.I. V-30.619.124 - Rol: desarrollador en la parte A
//   4. Raynuai Moran - C.I. V-32.979.355 - Rol: desarrollador en la parte B
//   5. Angel Rivas - C.I. V-32.517.670 - Rol: Apoyo en la parte B
//   6. Leo Chogo  - C.I. V-32.604.493 - Rol: Apoyo en la parte A
// ====================================================================

class PilaAcciones
{
private:
    struct Nodo
    {
        Accion dato;
        std::unique_ptr<Nodo> siguiente;

        explicit Nodo(const Accion& accion);
    };

    std::unique_ptr<Nodo> cima_;
    std::size_t cantidad;

public:
    PilaAcciones();
    PilaAcciones(const PilaAcciones& otra);
    PilaAcciones& operator=(const PilaAcciones& otra);
    PilaAcciones(PilaAcciones&& otra) noexcept;
    PilaAcciones& operator=(PilaAcciones&& otra) noexcept;
    ~PilaAcciones() = default;

    bool vacia() const;
    std::size_t tamanio() const;

    bool apilar(const Accion& accion);
    bool desapilar();

    const Accion* cima() const;
    Accion* cima();

    void limpiar();

    std::vector<Accion> aVector() const;

    const Accion* buscarPorId(int id) const;
    Accion* buscarPorId(int id);

    bool eliminarPorId(int id);
    bool moverAlCima(int id);
    void invertir();

    bool guardarCSV(const std::string& ruta) const;
    bool cargarCSV(const std::string& ruta);
};
