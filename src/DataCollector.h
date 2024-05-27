
#ifndef IOTGUARD_DATACOLLECTOR_H
#define IOTGUARD_DATACOLLECTOR_H

#include <string>
#include <map>
#include <vector>
#include <cpr/cpr.h>

namespace iotguard {

    struct RequestData {
        std::string id;
        std::string url;
        std::map<std::basic_string<char>, std::basic_string<char>, cpr::CaseInsensitiveCompare> headers;
        std::string body;
    };

///Data Collector отправляет REST-запросы к IoT-устройствам и получает данные.

    class DataCollector {
        std::string device;
        std::string host_address;
        std::tuple<std::string, std::string> user_auth;
        std::vector<RequestData> requests;
        std::string request_config;

        bool InitFromFile(const std::string& file_request);

    public:
        DataCollector() = delete;

        explicit DataCollector(const std::string& config_file);

        void GetData();

    };

} // iotguard

#endif //IOTGUARD_DATACOLLECTOR_H
