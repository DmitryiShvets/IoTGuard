#include <iostream>
#include <regex>
#include <fstream>

#include "DataParser.h"
#include "pugixml.hpp"
#include "nlohmann/json.hpp"
#include "Stuff.h"


using json = nlohmann::json;

namespace iotguard {

    void to_json(json &j, const LogEntry &p) {
        j = json{{"type",      p.type},
                 {"data",      p.data},
                 {"timestamp", p.timestamp},
                 {"hash",      p.hash}};
    }

    void from_json(const json &j, LogEntry &p) {
        j.at("type").get_to(p.type);
        j.at("data").get_to(p.data);
        j.at("timestamp").get_to(p.timestamp);
        j.at("hash").get_to(p.hash);
    }

    LogEntry LogEntry::from_json(const json &j) {
        return j.template get<iotguard::LogEntry>();
    }

    std::vector<LogEntry> HttpdParser::parse(const std::string &file) {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(file.c_str());

        if (!result) {
            std::cerr << "error parse " << file << " file \n";
            return {};
        }

        pugi::xml_node content = doc.child("file-content");
        std::string timestamp = content.attribute("timestamp").value();

        std::vector<std::string> lines = parse_lines(content.child_value());
        if (lines.size() <= offset) return {};
        std::vector<LogEntry> parse_result;

        std::vector<std::regex> reg;
        reg.emplace_back("httpd: 2: (Add filter) (.*)");
        reg.emplace_back("httpd: 2: (Activating module) (.*)");
        reg.emplace_back("httpd: [2|0]: (GET) (.*)");
        reg.emplace_back("httpd: [2|0]: (POST) (.*)");
        reg.emplace_back("httpd: 2: (Add handler) (.*)");
        reg.emplace_back("httpd: 2: (requestCompleted) (.*)");
        reg.emplace_back("httpd: 2: (reportFailure): (.*)");
        reg.emplace_back("httpd: 2: (Error): (.*)");
        reg.emplace_back("httpd: 2: (CreateMGD): (.*)");
        reg.emplace_back("httpd: 0: (parseJuniperDirectives) : (.*)");

        for (const auto &item: reg) {
            copy_if_match(parse_result, lines, item, timestamp);
            remove_if_match(lines, item);
        }

        std::regex regex_other("httpd: [2|1|0]: (.*)");
        copy_if_match(parse_result, lines, regex_other, timestamp);
        remove_if_match(lines, regex_other);

        if (learning_mode) {
            json httpd;
            for (const auto &item: parse_result) {
                httpd.emplace_back(item);
            }
            std::filesystem::create_directories(std::format(R"(reference\juniper)"));
            std::ofstream f(R"(reference\juniper\httpd.json)");
            f << httpd.dump(4);
            f.close();
        }
        offset += parse_result.size();
        return parse_result;
    }

    void HttpdParser::remove_if_match(std::vector<std::string> &input, const std::regex &regex) const {
        auto start = input.begin();
        std::advance(start, offset);
        input.erase(std::remove_if(start, input.end(),
                                   [&regex](const std::string &line) {
                                       return std::regex_match(line, regex);
                                   }), input.end());
    }


    void HttpdParser::copy_if_match(std::vector<LogEntry> &result, const std::vector<std::string> &input,
                                    const std::regex &regex, const std::string &timestamp) {
        auto start = input.begin();
        std::advance(start, offset);
        std::smatch pieces_match;
        for (auto it = start; it != input.end(); it++) {
            if (std::regex_match(*it, pieces_match, regex)) {
                size_t hash = hasher(*it);
                if (pieces_match.size() > 2) result.emplace_back(pieces_match[1], pieces_match[2], timestamp, hash);
                else result.emplace_back("Other", pieces_match[1], timestamp, hash);
            }
        }
    }

    HttpdParser::HttpdParser(bool learn_mode, bool use_cache) : learning_mode(learn_mode){
        std::filesystem::path file{R"(configs\parser-httpd-config.json)"};

        if (!use_cache || !std::filesystem::exists(file)) {
            offset = 0;
            return;
        }

        std::ifstream config_file(file);
        json config = json::parse(config_file, nullptr, false);
        config_file.close();

        if (config.is_discarded()) {
            offset = 0;
            return;
        }

        offset = config["offset"];
    }

    HttpdParser::~HttpdParser() {
        std::filesystem::path file{R"(configs\parser-httpd-config.json)"};

        std::ofstream config_file(file);
        json config;

        config["offset"] = offset;

        config_file << config.dump(4);
        config_file.close();


    }

    std::vector<LogEntry> ProcessParser::parse(const std::string &file) {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(file.c_str());

        if (!result) {
            std::cerr << "error parse " << file << " file \n";
            return {};
        }

        pugi::xml_node content = doc.child("system-process-information");
        std::string timestamp = content.attribute("timestamp").value();

        pugi::xml_node processes = content.child("process-information");

        std::vector<LogEntry> parse_result;

        for (pugi::xml_node proc = processes.first_child(); proc; proc = proc.next_sibling()) {
            std::string data = proc.child("command").text().get();
            size_t hash = hasher(data);
            parse_result.emplace_back("process", data, timestamp, hash);
        }

        if (learning_mode) {
            json proc;
            for (const auto &item: parse_result) {
                proc.emplace_back(item);
            }
            std::filesystem::create_directories(std::format(R"(reference\juniper)"));
            std::ofstream f(R"(reference\juniper\process.json)");
            f << proc.dump(4);
            f.close();
        }

        return parse_result;
    }

    ProcessParser::ProcessParser(bool learn_mode) : learning_mode(learn_mode) {}

    std::vector<std::string> parse_lines(const std::string &data) {
        std::vector<std::string> lines;
        std::istringstream stream(data);
        std::string line;

        while (std::getline(stream, line)) {
            if (!line.empty())lines.push_back(line);
        }

        return lines;
    }

    std::vector<LogEntry> DataParser::Parse(const std::string &data) {
        if (!parser) {
            return {};
        }
        return parser->parse(data);
    }
} // iotguard