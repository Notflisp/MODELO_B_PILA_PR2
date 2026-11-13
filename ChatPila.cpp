#include "ChatPila.hpp"

#include <utility>
#include <algorithm>
// ====================================================================
// PRACTICA EVALUADA 03 - MODELO B (PILA ENLAZADA)D
// Integrantes:
//   1. Mathias Molina - C.I. V-32.598.208 - Rol: header y desarrollador en la parte C
//   2. Miguel Andrade - C.I. V-31.905.358 - Rol: desarrollador en la parte B
//   3. Salomé Rondón - C.I. V-30.619.124 - Rol: desarrollador en la parte B
//   4. Raynuai Moran - C.I. V-32.979.355 - Rol: desarrollador en la parte A
//   5. Angel Rivas - C.I. V-32.517.670 - Rol: Apoyo en la parte A
//   6. Leo Chogo  - C.I. V-32.604.493 - Rol: Apoyo en la parte B
// ====================================================================
// ====================================================================
// Completen la integracion de las pilas con el chat.
// ====================================================================

ChatPila::ChatPila(int id, std::string nombre, std::vector<std::string> participantes)
    : Chat(id, std::move(nombre), std::move(participantes))
{
}

bool ChatPila::enviarMensaje(TextMessage mensaje)
{
    if (!isParticipant(mensaje.getSender()) ||
        buscarMensaje(mensaje.getId()) != nullptr ||
        mensaje.getStatus() != MessageStatus::Pending) {
        return false;
    }

    mensajes.push_back(mensaje); //guarda el mensaje
    pilaRehacer.limpiar(); //borrar la pila de acciones deshechas
    pila.apilar(Accion(TipoAccion::Enviar, mensaje)); //registra el mensaje
    return true;
}

bool ChatPila::editarMensaje(int id, const std::string& nuevoTexto)
{
    TextMessage* msg = buscarMensaje(id);
    if (msg == nullptr || nuevoTexto.empty()) {
        return false;
    }
    std::string textoAnterior = msg->getContent();
    msg->setContent(nuevoTexto);
    pilaRehacer.limpiar();
    pila.apilar(Accion(TipoAccion::Editar, *msg, textoAnterior));
    return true;
}

bool ChatPila::eliminarMensaje(int id)
{
    TextMessage* msg = buscarMensaje(id);
    if (msg == nullptr) {
        return false;
    }

    TextMessage mensajeEliminado(*msg);
    mensajes.erase(
        std::remove_if(mensajes.begin(), mensajes.end(),
            [id](const TextMessage& mensaje)
            {
                return mensaje.getId() == id;
            }),
        mensajes.end());

    pilaRehacer.limpiar();
    pila.apilar(Accion(TipoAccion::Eliminar, mensajeEliminado));
    return true;
}

bool ChatPila::deshacer()
{
    const Accion* cimaAccion = pila.cima();
    if (cimaAccion == nullptr)
    {
        return false;
    }

    const Accion accion = *cimaAccion;
    const int id = accion.getMensaje().getId();
    auto mensaje = mensajes.begin();
    while (mensaje != mensajes.end() && mensaje->getId() != id)
    {
        ++mensaje;
    }

    switch (accion.getTipo())
    {
    case TipoAccion::Enviar:
        if (mensaje == mensajes.end())
        {
            return false;
        }
        break;
    case TipoAccion::Eliminar:
        break;
    case TipoAccion::Editar:
        if (mensaje == mensajes.end())
        {
            return false;
        }
        break;
    }

    if (!pilaRehacer.apilar(accion))
    {
        return false;
    }

    switch (accion.getTipo())
    {
    case TipoAccion::Enviar:
        mensajes.erase(mensaje);
        break;
    case TipoAccion::Eliminar:
        if (mensaje == mensajes.end())
        {
            mensajes.push_back(accion.getMensaje());
        }
        break;
    case TipoAccion::Editar:
        mensaje->setContent(accion.getTextoAnterior());
        break;
    }

    pila.desapilar();
    return true;
}

bool ChatPila::rehacer()
{
    const Accion* cimaAccion = pilaRehacer.cima();
    if (cimaAccion == nullptr)
    {
        return false;
    }

    const Accion accion = *cimaAccion;
    const int id = accion.getMensaje().getId();
    auto mensaje = mensajes.begin();
    while (mensaje != mensajes.end() && mensaje->getId() != id)
    {
        ++mensaje;
    }

    switch (accion.getTipo())
    {
    case TipoAccion::Enviar:
        if (mensaje == mensajes.end())
        {
            mensajes.push_back(accion.getMensaje());
        }
        break;
    case TipoAccion::Eliminar:
        if (mensaje != mensajes.end())
        {
            mensajes.erase(mensaje);
        }
        break;
    case TipoAccion::Editar:
        if (mensaje == mensajes.end())
        {
            return false;
        }
        mensaje->setContent(accion.getMensaje().getContent());
        break;
    }

    if (!pila.apilar(accion))
    {
        return false;
    }

    pilaRehacer.desapilar();
    return true;
}

const std::vector<TextMessage>& ChatPila::getMensajes() const
{
    return mensajes;
}

const PilaAcciones& ChatPila::getPila() const
{
    return pila;
}

const PilaAcciones& ChatPila::getPilaRehacer() const
{
    return pilaRehacer;
}

std::size_t ChatPila::cantidadMensajes() const //cantidad de mensajes en el chat
{
    //con size() aseguramos que retorne la cantidad total de los elementos en mensaje
    return mensajes.size();
}

const TextMessage* ChatPila::buscarMensaje(int id) const //retorna apuntador a mensajes o nullptr + cons
{
    //creamos un bucle para recorrer la cantidad de mensajes por sus indices, si en el mensaje coincide el ID retorna el apuntador a mensajes o nulptr
    for (std::size_t i = 0; i < mensajes.size(); ++i) {
        if (mensajes[i].getId() == id) {
            return &mensajes[i];
        }
    }
    return nullptr;
}

TextMessage* ChatPila::buscarMensaje(int id) //retorna apuntador a mensajes o nullptr
{
    //creamos un bucle para recorrer la cantidad de mensajes por sus indices, si en el mensaje coincide el ID retorna el apuntador a mensajes o nulptr
    for (std::size_t i = 0; i < mensajes.size(); ++i) {
        if (mensajes[i].getId() == id) {
            return &mensajes[i];
        }
    }
    return nullptr;
}
