#ifndef IOTGUARD_DATAPARSER_H
#define IOTGUARD_DATAPARSER_H

#include <string>
#include <vector>
#include "Stuff.h"


namespace iotguard {

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
        bool learning_mode;
        std::hash<std::string> hasher;

        void remove_if_match(std::vector<std::string> &input, const std::regex &regex) const;

        void copy_if_match(std::vector<LogEntry> &result,
                           const std::vector<std::string> &input, const std::regex &regex,
                           const std::string &timestamp);


    public:
        HttpdParser(bool learning_mode,bool use_cache);

        std::vector<LogEntry> parse(const std::string &data) override;

        ~HttpdParser() override;
    };

    // Парсер для process логов
    class ProcessParser : public IParser {
        bool learning_mode;
        std::hash<std::string> hasher;
    public:

        explicit ProcessParser(bool learning_mode);

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


#endif //IOTGUARD_DATAPARSER_H
