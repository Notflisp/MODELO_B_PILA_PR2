#pragma once

#include <string>

// Estados de un mensaje. Avanzan en un solo sentido:
// Pending -> Sent -> Delivered -> Opened
enum class MessageStatus
{
    Pending,
    Sent,
    Delivered,
    Opened
};

// Conversion al texto que se guarda en el CSV ("pending", "sent", ...).
std::string toString(MessageStatus status);
MessageStatus messageStatusFromString(const std::string& text);
