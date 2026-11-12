#include "PilaAcciones.hpp"

#include <utility>

// ====================================================================
// PRACTICA EVALUADA 03 - MODELO B (PILA ENLAZADA)
// Integrantes:
//   1. Mathias Molina - C.I. V-32.598.208 - Rol: header y desarrollador en la parte C
//   2. Miguel Andrade - C.I. V-31.905.358 - Rol: desarrollador en la parte B
//   3. Salomé Rondón - C.I. V-30.619.124 - Rol: desarrollador en la parte B
//   4. Raynuai Moran - C.I. V-32.979.355 - Rol: desarrollador en la parte A
//   5. Angel Rivas - C.I. V-32.517.670 - Rol: Apoyo en la parte A
//   6. Leo Chogo  - C.I. V-32.604.493 - Rol: Apoyo en la parte B
// ====================================================================
// ====================================================================
// Completen la implementacion de esta estructura.
// ====================================================================

PilaAcciones::Nodo::Nodo(const Accion& accion)
    : dato(accion), siguiente(nullptr)
{
}

PilaAcciones::PilaAcciones()
    : cima_(nullptr), cantidad(0)
{
}

PilaAcciones::PilaAcciones(const PilaAcciones& otra)
    : cima_(nullptr), cantidad(0)
{
    // TODO: completar.
    (void)otra;
}

PilaAcciones& PilaAcciones::operator=(const PilaAcciones& otra)
{
    // TODO: completar.
    (void)otra;
    return *this;
}

PilaAcciones::PilaAcciones(PilaAcciones&& otra) noexcept
    : cima_(nullptr), cantidad(0)
{
    // TODO: completar.
    (void)otra;
}

PilaAcciones& PilaAcciones::operator=(PilaAcciones&& otra) noexcept
{
    // TODO: completar.
    (void)otra;
    return *this;
}

bool PilaAcciones::vacia() const
{
    // TODO: completar.
    return false;
}

std::size_t PilaAcciones::tamanio() const
{
    // TODO: completar.
    return 0;
}

bool PilaAcciones::apilar(const Accion& accion)
{
    // TODO: completar.
    (void)accion;
    return false;
}

bool PilaAcciones::desapilar()
{
    // TODO: completar.
    return false;
}

const Accion* PilaAcciones::cima() const
{
    // TODO: completar.
    return nullptr;
}

Accion* PilaAcciones::cima()
{
    // TODO: completar.
    return nullptr;
}

void PilaAcciones::limpiar()
{
    // TODO: completar.
}

std::vector<Accion> PilaAcciones::aVector() const
{
    // TODO: completar.
    return {};
}

const Accion* PilaAcciones::buscarPorId(int id) const
{
    // TODO: completar.
    (void)id;
    return nullptr;
}

Accion* PilaAcciones::buscarPorId(int id)
{
    // TODO: completar.
    (void)id;
    return nullptr;
}

bool PilaAcciones::eliminarPorId(int id)
{
    // TODO: completar.
    (void)id;
    return false;
}

bool PilaAcciones::moverAlCima(int id)
{
    // TODO: completar.
    (void)id;
    return false;
}

void PilaAcciones::invertir()
{
    // TODO: completar.
}

bool PilaAcciones::guardarCSV(const std::string& ruta) const
{
    // TODO: completar.
    (void)ruta;
    return false;
}

bool PilaAcciones::cargarCSV(const std::string& ruta)
{
    // TODO: completar.
    (void)ruta;
    return false;
}
