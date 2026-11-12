#include "MessageStatus.hpp"

std::string toString(MessageStatus status)
{
    switch (status)
    {
    case MessageStatus::Pending:
        return "pending";
    case MessageStatus::Sent:
        return "sent";
    case MessageStatus::Delivered:
        return "delivered";
    case MessageStatus::Opened:
        return "opened";
    }

    return "pending";
}

MessageStatus messageStatusFromString(const std::string& text)
{
    if (text == "sent" || text == "Sent" || text == "Enviado")
    {
        return MessageStatus::Sent;
    }
    if (text == "delivered" || text == "Delivered" || text == "Entregado")
    {
        return MessageStatus::Delivered;
    }
    if (text == "opened" || text == "Opened" || text == "Abierto")
    {
        return MessageStatus::Opened;
    }
    return MessageStatus::Pending;
}
