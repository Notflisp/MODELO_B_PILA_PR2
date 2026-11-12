#pragma once

#include <string>
#include <vector>

#include "Chat.hpp"
#include "PilaAcciones.hpp"

// ====================================================================
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
// ====================================================================

class ChatPila : public Chat
{
private:
    std::vector<TextMessage> mensajes;
    PilaAcciones pila;
    PilaAcciones pilaRehacer;

public:
    ChatPila(int id, std::string nombre, std::vector<std::string> participantes);

    bool enviarMensaje(TextMessage mensaje);
    bool editarMensaje(int id, const std::string& nuevoTexto);
    bool eliminarMensaje(int id);

    bool deshacer();
    bool rehacer();

    const std::vector<TextMessage>& getMensajes() const;
    const PilaAcciones& getPila() const;
    const PilaAcciones& getPilaRehacer() const;
    std::size_t cantidadMensajes() const;

    const TextMessage* buscarMensaje(int id) const;
    TextMessage* buscarMensaje(int id);
};
