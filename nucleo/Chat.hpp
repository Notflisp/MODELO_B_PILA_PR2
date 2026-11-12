#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "User.hpp"

// Chat basico con participantes.
// Version reducida de Chat de MessagingApp: conserva titulo, participantes y
// validaciones de pertenencia, pero NO almacena mensajes. Cada modelo guarda
// sus mensajes en la estructura evaluada (lista, pila o cola).
class Chat
{
public:
    Chat(int id, std::string title, std::vector<std::shared_ptr<User>> participants);
    Chat(int id, std::string title, std::vector<std::string> participantIds);

    int getId() const;
    const std::string& getTitle() const;
    const std::vector<std::shared_ptr<User>>& getParticipants() const;

    bool isParticipant(const std::string& userId) const;
    bool isParticipant(const std::shared_ptr<User>& user) const;

    bool addParticipant(const std::shared_ptr<User>& user);
    bool addParticipant(const std::string& userId);
    bool removeParticipant(const std::string& userId);

    std::size_t getParticipantCount() const;
    std::string getType() const;

private:
    int id;
    std::string title;
    std::vector<std::shared_ptr<User>> participants;
};
