#pragma once

#include <string>

#include "TextMessage.hpp"

// Tipo de accion guardada para deshacer y rehacer.
enum class TipoAccion
{
    Enviar,
    Eliminar,
    Editar
};

// Accion reversible sobre un mensaje.
class Accion
{
private:
    TipoAccion tipo;
    TextMessage mensaje;
    std::string textoAnterior;

public:
    Accion(TipoAccion tipo, const TextMessage& mensaje, const std::string& textoAnterior = "");

    TipoAccion getTipo() const;
    const TextMessage& getMensaje() const;
    const std::string& getTextoAnterior() const;
    std::string getTipoTexto() const;

    std::string toCSV() const;
    static Accion fromCSV(const std::string& linea);
};
