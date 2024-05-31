#include "DataCollector.h"

#include <utility>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <ctime>
// for convenience
using json = nlohmann::json;

#include <filesystem>
#include "pugixml.hpp"

namespace iotguard {


    bool DataCollector::GetData() {
        for (RequestData &request: requests) {
            cpr::Response response = cpr::Post(cpr::Url{host_address + request.url},
                                               cpr::Authentication{std::get<0>(user_auth),
                                                                   std::get<1>(user_auth),
                                                                   cpr::AuthMode::BASIC},
                                               cpr::Header{request.headers},
                                               cpr::Body{request.body}
            );
            if (response.status_code == 200) {
                //time_t now = time(0);
                std::time_t now = std::time(nullptr);
                std::string file_path = std::format(R"(logs\{}\{}\{}.xml)", device, request.id, now);

                pugi::xml_document doc;
                pugi::xml_parse_result result = doc.load_string(response.text.c_str());

                if (result) {
                    std::string timestamp = std::ctime(&now);
                    timestamp.erase(std::remove(timestamp.begin(), timestamp.end(), '\n'), timestamp.end());
                    doc.document_element().append_attribute("timestamp").set_value(timestamp.c_str());

                    doc.save_file(file_path.c_str());
                } else {
                    std::cerr << "XML  parsed with errors, attr value: [" << doc.child("node").attribute("attr").value()
                              << "]\n";
                    std::cerr << "Error description: " << result.description() << "\n";
                    std::cerr << "Error offset: " << result.offset << " (error at [..."
                              << (response.text.c_str() + result.offset) << "]\n";
                    return false;
                }
            } else {
                std::cerr << "Error fetch data: " << request.id << " response status code: " << response.status_code
                          << "\n";
                return false;
            }
        }
        return true;
    }

    DataCollector::DataCollector(const std::string &config_file) {
        std::ifstream file(config_file);
        json config = json::parse(file, nullptr, false);
        file.close();
        if (config.is_discarded()) {
            throw std::invalid_argument(std::format("file {} parse error", config_file));
        }
        user_auth = std::make_tuple(config["user"]["login"], config["user"]["pass"]);
        host_address = config["host"];
        device = config["device"];
        request_config = config["requests-config"];

        if (!InitFromFile(request_config))
            throw std::invalid_argument(std::format("file {} parse error", request_config));
    }

    bool DataCollector::InitFromFile(const std::string &file_request) {
        std::ifstream file(file_request);
        json config = json::parse(file, nullptr, false);
        file.close();
        if (config.is_discarded()) {
            return false;
        }
        for (auto &[key, data]: config.items()) {
            RequestData tmp{};
            tmp.id = key;
            tmp.url = data["url"];
            tmp.body = data["body"];
            for (auto &[header, value]: data["headers"].items()) {
                tmp.headers[header] = value;
            }
            requests.push_back(std::move(tmp));
            std::filesystem::create_directories(std::format(R"(logs\{}\{})", device, key));
        }
        file.close();
        return true;
    }
} // iotguard