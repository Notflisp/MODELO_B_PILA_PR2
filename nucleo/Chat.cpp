#include "Chat.hpp"

#include <utility>

namespace
{
std::shared_ptr<User> makeUser(const std::string& id)
{
    return std::make_shared<User>(id, id);
}
} // namespace

Chat::Chat(int id, std::string title, std::vector<std::shared_ptr<User>> participants)
    : id(id > 0 ? id : 1),
      title(title.empty() ? "Chat sin titulo" : std::move(title))
{
    for (const std::shared_ptr<User>& participant : participants)
    {
        addParticipant(participant);
    }
}

Chat::Chat(int id, std::string title, std::vector<std::string> participantIds)
    : Chat(id, std::move(title), std::vector<std::shared_ptr<User>>{})
{
    for (const std::string& participantId : participantIds)
    {
        addParticipant(participantId);
    }
}

int Chat::getId() const
{
    return id;
}

const std::string& Chat::getTitle() const
{
    return title;
}

const std::vector<std::shared_ptr<User>>& Chat::getParticipants() const
{
    return participants;
}

bool Chat::isParticipant(const std::string& userId) const
{
    for (const std::shared_ptr<User>& participant : participants)
    {
        if (participant && participant->getId() == userId)
        {
            return true;
        }
    }
    return false;
}

bool Chat::isParticipant(const std::shared_ptr<User>& user) const
{
    if (!user)
    {
        return false;
    }
    return isParticipant(user->getId());
}

bool Chat::addParticipant(const std::shared_ptr<User>& user)
{
    if (!user || isParticipant(user->getId()))
    {
        return false;
    }

    participants.push_back(user);
    return true;
}

bool Chat::addParticipant(const std::string& userId)
{
    if (userId.empty())
    {
        return false;
    }
    return addParticipant(makeUser(userId));
}

bool Chat::removeParticipant(const std::string& userId)
{
    if (participants.size() <= 2)
    {
        return false;
    }

    for (auto it = participants.begin(); it != participants.end(); ++it)
    {
        if (*it && (*it)->getId() == userId)
        {
            participants.erase(it);
            return true;
        }
    }

    return false;
}

std::size_t Chat::getParticipantCount() const
{
    return participants.size();
}

std::string Chat::getType() const
{
    return participants.size() == 2 ? "direct" : "group";
}
