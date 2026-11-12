#pragma once

#include <cstddef>
#include <string>
#include <vector>

// Utilidades CSV inspiradas en MessagingApp/utils/CsvUtils.
// La version de la practica agrega soporte para campos entre comillas dobles,
// porque el contenido de un mensaje puede contener comas.
namespace CsvUtils
{

// Divide una linea CSV respetando campos entre comillas dobles.
// Ejemplo: 1,"hola, mundo",sent  ->  [1, hola, mundo, sent]
inline std::vector<std::string> splitCSV(const std::string& line)
{
    std::vector<std::string> fields;
    std::string field;
    bool inQuotes = false;

    for (std::size_t i = 0; i < line.size(); ++i)
    {
        const char c = line[i];

        if (c == '"')
        {
            if (inQuotes && i + 1 < line.size() && line[i + 1] == '"')
            {
                field += '"';
                ++i;
            }
            else
            {
                inQuotes = !inQuotes;
            }
        }
        else if (c == ',' && !inQuotes)
        {
            fields.push_back(field);
            field.clear();
        }
        else
        {
            field += c;
        }
    }

    fields.push_back(field);
    return fields;
}

// Alias con el nombre clasico de MessagingApp para separar por un delimitador.
inline std::vector<std::string> split(const std::string& text, char delimiter)
{
    std::vector<std::string> parts;
    std::string current;

    for (char c : text)
    {
        if (c == delimiter)
        {
            parts.push_back(current);
            current.clear();
        }
        else
        {
            current += c;
        }
    }

    parts.push_back(current);
    return parts;
}

inline std::string join(const std::vector<std::string>& parts, char delimiter)
{
    std::string result;
    for (std::size_t i = 0; i < parts.size(); ++i)
    {
        if (i > 0)
        {
            result += delimiter;
        }
        result += parts[i];
    }
    return result;
}

} // namespace CsvUtils

