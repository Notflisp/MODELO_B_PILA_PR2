#include "Accion.hpp"
#include "CSV.hpp"

#include <iomanip>
#include <sstream>

// Accion es el dato que guarda la pila. Se entrega implementada para que el
// estudiante concentre su esfuerzo en la estructura enlazada.

Accion::Accion(TipoAccion tipo, const TextMessage& mensaje, const std::string& textoAnterior)
    : tipo(tipo), mensaje(mensaje), textoAnterior(textoAnterior)
{
}

TipoAccion Accion::getTipo() const
{
    return tipo;
}

const TextMessage& Accion::getMensaje() const
{
    return mensaje;
}

const std::string& Accion::getTextoAnterior() const
{
    return textoAnterior;
}

std::string Accion::getTipoTexto() const
{
    switch (tipo)
    {
    case TipoAccion::Enviar:
        return "Enviar";
    case TipoAccion::Eliminar:
        return "Eliminar";
    case TipoAccion::Editar:
        return "Editar";
    }

    return "Enviar";
}

std::string Accion::toCSV() const
{
    std::ostringstream salida;
    salida << getTipoTexto() << ','
           << mensaje.toCSV() << ','
           << std::quoted(textoAnterior);
    return salida.str();
}

Accion Accion::fromCSV(const std::string& linea)
{
    const std::vector<std::string> campos = CsvUtils::splitCSV(linea);
    if (campos.size() < 6)
    {
        return Accion(TipoAccion::Enviar, TextMessage(1, "desconocido", "(vacio)"));
    }

    TipoAccion tipoLeido = TipoAccion::Enviar;
    if (campos[0] == "Eliminar")
    {
        tipoLeido = TipoAccion::Eliminar;
    }
    else if (campos[0] == "Editar")
    {
        tipoLeido = TipoAccion::Editar;
    }

    std::ostringstream lineaMensaje;
    lineaMensaje << campos[1] << ','
                 << std::quoted(campos[2]) << ','
                 << std::quoted(campos[3]) << ','
                 << campos[4];

    return Accion(tipoLeido, TextMessage::fromCSV(lineaMensaje.str()), campos[5]);
}
