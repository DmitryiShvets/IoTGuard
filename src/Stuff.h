//
// Created by Dmitry on 30.05.2024.
//

#ifndef IOTGUARD_STUFF_H
#define IOTGUARD_STUFF_H

#include <string_view>
#include <vector>
#include <regex>
#include <map>
#include <cpr/cpr.h>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace iotguard {

    struct LogEntry {
        std::string type;
        std::string data;
        std::string timestamp;
        std::size_t hash;

        bool operator==(const LogEntry &other) const {
            return type == other.type && hash == other.hash;
        }

        static LogEntry from_json(const json &j);
    };

    struct RequestData {
        std::string id;
        std::string url;
        std::map<std::basic_string<char>, std::basic_string<char>, cpr::CaseInsensitiveCompare> headers;
        std::string body;
    };

    enum IMPORTANCE_LEVEL {
        INFO,      // Информационные сообщения
        UNDEFINED,    // Нераспознанные, требующие внимания
        WARNING,   // Предупреждения
        ERR,     // Ошибки
        CRITICAL,  // Критические ошибки
        THRESHOLD
    };
    constexpr std::string_view LEVEL[] =
            {
                    "INFO",
                    "UNDEFINED",
                    "WARNING",
                    "ERR",
                    "CRITICAL",
                    "THRESHOLD",
            };


    struct DetectionRule {
        std::string entry_type; // Тип аномалии
        std::regex signature;  // Сигнатура аномалии
        IMPORTANCE_LEVEL level; // Уровень важности правила
        int threshold; // Порог срабатывания правила
    };

    struct AnomalyEntry {
        friend std::ostream &operator<<(std::ostream &os, const AnomalyEntry &entry);

        std::string type;  // Тип аномалии
        std::string data;  // Сообщение лога, связанное с аномалией
        IMPORTANCE_LEVEL level; // Уровень важности аномалии
        std::string timestamp; // Время обнаружения аномалии
        size_t hash; // Хеш аномалии
    };

}

namespace std {
    template<>
    struct hash<iotguard::LogEntry> {
        std::size_t operator()(const iotguard::LogEntry &entry) const {
            return entry.hash;
        }
    };
}

#endif //IOTGUARD_STUFF_H
