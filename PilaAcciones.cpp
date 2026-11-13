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
    : cima_(nullptr), cantidad(otra.cantidad)
{
    if (!otra.cima_) return;

    // Copiamos el primer nodo usando make_unique
    cima_ = std::make_unique<Nodo>(otra.cima_->dato);

    Nodo* actualOriginal = otra.cima_->siguiente.get();
    Nodo* ultimoNuevo = cima_.get();

    while (actualOriginal != nullptr)
    {
        ultimoNuevo->siguiente = std::make_unique<Nodo>(actualOriginal->dato);
        ultimoNuevo = ultimoNuevo->siguiente.get();
        actualOriginal = actualOriginal->siguiente.get();
    }
}

PilaAcciones& PilaAcciones::operator=(const PilaAcciones& otra)
{
    // TODO: completar.
    if(this != &otra)
    {
        limpiar();
        if(otra.cima_ != nullptr)
        {
            cima_ = std::make_unique<Nodo>(otra.cima_->dato);
            cantidad = otra.cantidad;
            
            Nodo* actualOtra = otra.cima_->siguiente.get();
            Nodo* ultimoNuevo = cima_.get();
            
            while(actualOtra != nullptr)
            {
                ultimoNuevo->siguiente = std::make_unique<Nodo>(actualOtra->dato);
                ultimoNuevo = ultimoNuevo->siguiente.get();
                actualOtra = actualOtra->siguiente.get();
            }
        }
    }
    return *this;
}

PilaAcciones::PilaAcciones(PilaAcciones&& otra) noexcept
    : cima_(std::move(otra.cima_)), cantidad(otra.cantidad)
{
    otra.cantidad = 0;
}

PilaAcciones& PilaAcciones::operator=(PilaAcciones&& otra) noexcept
{
    // TODO: completar.
    if (this != &otra)
    {
        limpiar();
        cima_ = std::move(otra.cima_);
        cantidad = otra.cantidad;
        otra.cantidad = 0;
    }
    return *this;
}     
    
bool PilaAcciones::vacia() const
{
    // TODO: completar.
    return cima_ == nullptr;
}

std::size_t PilaAcciones::tamanio() const
{
    // TODO: completar.
    return cantidad;
}

bool PilaAcciones::apilar(const Accion& accion)
{
    // TODO: completar.
    auto nuevo = std::make_unique<Nodo>(accion);
    nuevo->siguiente = std::move(cima_);
    cima_ = std::move(nuevo);
    cantidad++;
    return true;
}

bool PilaAcciones::desapilar()
{
    // TODO: completar.
    if (cima_ == nullptr)
    {
        return false;
    }
    
    cima_ = std::move(cima_->siguiente);
    cantidad--;
    return true;
}

Accion* PilaAcciones::cima()
{
    // TODO: completar.
    if (cima_ == nullptr)
        {
            return nullptr;
        }
    return &(cima_->dato);
}

void PilaAcciones::limpiar()
{
    // TODO: completar.
    while (cima_ != nullptr)
    {
        desapilar();
    }
}

std::vector<Accion> PilaAcciones::aVector() const
{
    // TODO: completar.
   std::vector<Accion> vec;
    Nodo* actual = cima_.get();
    while (actual != nullptr)
    {
        vec.push_back(actual->dato);
        actual = actual->siguiente.get();
    }
    return vec;
}

const Accion* PilaAcciones::buscarPorId(int id) const
{
    Nodo* actual = cima_.get();
    while (actual != nullptr)
    {
        if (actual->dato.mensaje.getId() == id)
        {
            return &(actual->dato);
        }
        actual = actual->siguiente.get();
    }
    return nullptr;
}

Accion* PilaAcciones::buscarPorId(int id)
{
    return const_cast<Accion*>(static_cast<const PilaAcciones*>(this)->buscarPorId(id));
}

bool PilaAcciones::eliminarPorId(int id)
{
    if (vacia()) return false;
    
    std::vector<Accion> temp = aVector();
    bool encontrado = false;
    std::vector<Accion> nuevos;
    
    for (const auto& acc : temp)
    {
        if (acc.id == id)
        {
            encontrado = true;
        }
        else
        {
            nuevos.push_back(acc);
        }
    }
    
    if (!encontrado) return false;
    
    limpiar();
    for (int i = nuevos.size() - 1; i >= 0; --i)
    {
        apilar(nuevos[i]);
    }
    return true;
}

bool PilaAcciones::moverAlCima(int id)
{
    if (vacia()) return false;
    
    std::vector<Accion> temp = aVector();
    int idxEncontrado = -1;
    
    for (size_t i = 0; i < temp.size(); ++i)
    {
        if (temp[i].getId() == id)
        {
            idxEncontrado = i;
            break;
        }
    }
    
    if (idxEncontrado == -1) return false;
    
    Accion encontrada = temp[idxEncontrado];
    temp.erase(temp.begin() + idxEncontrado);
    
    limpiar();
    for (int i = temp.size() - 1; i >= 0; --i)
    {
        apilar(temp[i]);
    }
    apilar(encontrada);
    return true;
}

void PilaAcciones::invertir()
{
    if (vacia()) return;
    std::vector<Accion> temp = aVector();
    limpiar();
    for (const auto& acc : temp)
    {
        apilar(acc);
    }
}

bool PilaAcciones::guardarCSV(const std::string& ruta) const
{
    // TODO: completar.
    std::ofstream archivo(ruta);
    if (!archivo.is_open())
    {
        return false;
    }
    
    Nodo* actual = cima_.get();
    while (actual != nullptr)
    {
        archivo << actual->dato.getId() << "\n";
        actual = actual->siguiente.get();
    }
    
    archivo.close();
    return true;
}

bool PilaAcciones::cargarCSV(const std::string& ruta)
{
    // TODO: completar.
    std::ifstream archivo(ruta);
    if (!archivo.is_open())
    {
        return false;
    }
    
    limpiar();
    std::string linea;
    while (std::getline(archivo, linea))
    {
        if (linea.empty())
        {
            continue;
        }
    }
    
    archivo.close();
    return true;
}
