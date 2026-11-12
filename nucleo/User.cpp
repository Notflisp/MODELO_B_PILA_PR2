#include "User.hpp"

#include <utility>

User::User(std::string id, std::string name, std::string lastName)
    : externalId(id.empty() ? "sin-id" : std::move(id)),
      name(name.empty() ? "Sin nombre" : std::move(name)),
      lastName(std::move(lastName))
{
}

const std::string& User::getId() const
{
    return externalId;
}

const std::string& User::getExternalId() const
{
    return externalId;
}

const std::string& User::getName() const
{
    return name;
}

const std::string& User::getLastName() const
{
    return lastName;
}

std::string User::getFullName() const
{
    if (lastName.empty())
    {
        return name;
    }
    return name + " " + lastName;
}

void User::setName(const std::string& newName)
{
    if (!newName.empty())
    {
        name = newName;
    }
}

void User::setLastName(const std::string& newLastName)
{
    lastName = newLastName;
}
