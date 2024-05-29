#ifndef IOTGUARD_DATAPARSER_H
#define IOTGUARD_DATAPARSER_H

#include <string>
#include <vector>
#include <regex>

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

    std::vector<std::string> parse_lines(const std::string &data);

    // Базовый интерфейс для парсеров
    class IParser {
    public:
        virtual ~IParser() = default;

        virtual std::vector<LogEntry> parse(const std::string &data) = 0;
    };

    // Парсер для httpd логов
    class HttpdParser : public IParser {
        std::size_t offset;

        std::hash<std::string> hasher;

        void remove_if_match(std::vector<std::string> &input, const std::regex &regex) const;

        void copy_if_match(std::vector<LogEntry> &result,
                           const std::vector<std::string> &input, const std::regex &regex,
                           const std::string &timestamp);


    public:
        HttpdParser(bool use_cache);

        std::vector<LogEntry> parse(const std::string &data) override;

        ~HttpdParser() override;
    };

    // Парсер для process логов
    class ProcessParser : public IParser {
        std::hash<std::string> hasher;
    public:
        std::vector<LogEntry> parse(const std::string &data) override;
    };

    ///Data Parser распарсивает полученные данные и передает их в State Comparator.

    class DataParser {
    public:
        void SetParser(IParser *p) {
            this->parser = p;
        }

        std::vector<LogEntry> Parse(const std::string &data);

    private:
        IParser *parser = nullptr;

    };

} // iotguard

namespace std {
    template<>
    struct hash<iotguard::LogEntry> {
        std::size_t operator()(const iotguard::LogEntry &entry) const {
            return entry.hash;
        }
    };
}

#endif //IOTGUARD_DATAPARSER_H
