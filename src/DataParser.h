#ifndef IOTGUARD_DATAPARSER_H
#define IOTGUARD_DATAPARSER_H

#include <string>
#include <memory>
#include <vector>
#include <regex>

namespace iotguard {
    struct LogEntry {
        std::string type;
        std::string data;
        std::string timestamp;
        std::size_t hash;
    };

    std::vector<std::string> parse_lines(const std::string &data);

    // Базовый интерфейс для парсеров
    class IParser {
    public:
        virtual ~IParser() = default;

        virtual std::vector<LogEntry> parse(const std::string &data) = 0;
    };

    // Парсер для httpd логов
    class HttpdParser :   public IParser {
        std::size_t offset = 0;
        std::hash<std::string> hasher;
        void remove_if_match(std::vector<std::string> &input, const std::regex &regex) const;
        void copy_if_match(std::vector<LogEntry>&result, const std::vector<std::string> &input, const std::regex &regex, const std::string& timestamp);


    public:
        std::vector<LogEntry> parse(const std::string &data) override;
    };

    // Парсер для process логов
    class ProcessParser : public IParser {
        std::hash<std::string> hasher;
    public:
        std::vector<LogEntry> parse(const std::string &data) override;
    };

    ///Data Parser распарсивает полученные данные и передает их в State Comparator.

    class DataParser {

        void SetParser(std::unique_ptr<IParser> p) {
            this->parser = std::move(p);
        }

        void Parse(const std::string &data);

        std::unique_ptr<IParser> parser;
    public:
        void ParseData();
    };

} // iotguard

#endif //IOTGUARD_DATAPARSER_H
