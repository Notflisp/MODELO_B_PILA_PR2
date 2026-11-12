#pragma once

#include <string>

// Usuario del sistema de mensajeria.
// Version reducida de User de MessagingApp: conserva id, nombre, apellido y
// getFullName(), pero omite UUID, fechas y controladores para que la practica
// se concentre en la estructura de datos evaluada.
class User
{
private:
    std::string externalId;
    std::string name;
    std::string lastName;

public:
    User(std::string id, std::string name, std::string lastName = "");

    const std::string& getId() const;
    const std::string& getExternalId() const;
    const std::string& getName() const;
    const std::string& getLastName() const;
    std::string getFullName() const;

    void setName(const std::string& newName);
    void setLastName(const std::string& newLastName);
};
