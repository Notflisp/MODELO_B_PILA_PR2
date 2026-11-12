#pragma once

#include <memory>
#include <string>

#include "MessageStatus.hpp"
#include "User.hpp"

// Mensaje de texto del sistema de mensajeria.
// Version reducida de TextMessage de MessagingApp: conserva remitente,
// contenido, estado y transiciones markSent/markDelivered/markOpened, pero
// omite fechas y UUID para no desviar la atencion de la estructura evaluada.
class TextMessage
{
public:
    static constexpr const char* TYPE = "text";

    // Constructor principal: remitente como User compartido (estilo MessagingApp).
    TextMessage(int id, std::shared_ptr<User> sender, std::string content);

    // Constructor de comodidad: crea el User a partir de su id.
    TextMessage(int id, const std::string& senderId, std::string content);

    int getId() const;
    int getInternalId() const;

    std::shared_ptr<User> getSender() const;
    const std::string& getSenderId() const;

    const std::string& getContent() const;
    MessageStatus getStatus() const;
    std::string getStatusText() const;
    int getChatId() const;

    bool isSent() const;
    bool isDelivered() const;
    bool isOpened() const;

    // Transiciones validas: Pending -> Sent -> Delivered -> Opened.
    bool markSent(int chatId);
    bool markDelivered();
    bool markOpened();

    void setContent(const std::string& newContent);

    // Formato CSV: id,"remitente","contenido",estado
    std::string toCSV() const;
    static TextMessage fromCSV(const std::string& line);

private:
    int internalId;
    std::shared_ptr<User> sender;
    std::string content;
    MessageStatus status;
    int chatId;
};
