#include "TextMessage.hpp"

#include "CSV.hpp"

#include <iomanip>
#include <sstream>
#include <utility>

namespace
{
std::shared_ptr<User> makeSender(const std::string& senderId)
{
    return std::make_shared<User>(senderId, senderId);
}
} // namespace

TextMessage::TextMessage(int id, std::shared_ptr<User> sender, std::string content)
    : internalId(id > 0 ? id : 1),
      sender(sender ? std::move(sender) : makeSender("desconocido")),
      content(content.empty() ? "(vacio)" : std::move(content)),
      status(MessageStatus::Pending),
      chatId(0)
{
}

TextMessage::TextMessage(int id, const std::string& senderId, std::string content)
    : TextMessage(id, makeSender(senderId.empty() ? "desconocido" : senderId), std::move(content))
{
}

int TextMessage::getId() const
{
    return internalId;
}

int TextMessage::getInternalId() const
{
    return internalId;
}

std::shared_ptr<User> TextMessage::getSender() const
{
    return sender;
}

const std::string& TextMessage::getSenderId() const
{
    return sender->getId();
}

const std::string& TextMessage::getContent() const
{
    return content;
}

MessageStatus TextMessage::getStatus() const
{
    return status;
}

std::string TextMessage::getStatusText() const
{
    return toString(status);
}

int TextMessage::getChatId() const
{
    return chatId;
}

bool TextMessage::isSent() const
{
    return status == MessageStatus::Sent;
}

bool TextMessage::isDelivered() const
{
    return status == MessageStatus::Delivered;
}

bool TextMessage::isOpened() const
{
    return status == MessageStatus::Opened;
}

bool TextMessage::markSent(int chatId)
{
    if (status != MessageStatus::Pending)
    {
        return false;
    }

    status = MessageStatus::Sent;
    this->chatId = chatId;
    return true;
}

bool TextMessage::markDelivered()
{
    if (status != MessageStatus::Sent)
    {
        return false;
    }

    status = MessageStatus::Delivered;
    return true;
}

bool TextMessage::markOpened()
{
    if (status != MessageStatus::Delivered)
    {
        return false;
    }

    status = MessageStatus::Opened;
    return true;
}

void TextMessage::setContent(const std::string& newContent)
{
    if (!newContent.empty())
    {
        content = newContent;
    }
}

std::string TextMessage::toCSV() const
{
    std::ostringstream output;
    output << internalId << ','
           << std::quoted(getSenderId()) << ','
           << std::quoted(content) << ','
           << getStatusText();
    return output.str();
}

TextMessage TextMessage::fromCSV(const std::string& line)
{
    const std::vector<std::string> fields = CsvUtils::splitCSV(line);
    if (fields.size() < 4)
    {
        return TextMessage(1, "desconocido", "(vacio)");
    }

    int readId = 1;
    try
    {
        readId = std::stoi(fields[0]);
    }
    catch (...)
    {
        readId = 1;
    }

    TextMessage message(readId, fields[1], fields[2]);
    const MessageStatus readStatus = messageStatusFromString(fields[3]);

    if (readStatus == MessageStatus::Sent)
    {
        message.markSent(0);
    }
    else if (readStatus == MessageStatus::Delivered)
    {
        message.markSent(0);
        message.markDelivered();
    }
    else if (readStatus == MessageStatus::Opened)
    {
        message.markSent(0);
        message.markDelivered();
        message.markOpened();
    }

    return message;
}
