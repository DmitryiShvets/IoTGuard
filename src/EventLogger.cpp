#include "EventLogger.h"

namespace iotguard {

    void to_json(json &j, const AnomalyEntry &p) {
        j = json{{"type",       p.type},
                 {"data",       p.data},
                 {"importance", LEVEL[p.level]},
                 {"timestamp",  p.timestamp},
                 {"hash",       p.hash}};
    }

    void from_json(const json &j, AnomalyEntry &p) {
        j.at("type").get_to(p.type);
        j.at("data").get_to(p.data);
        j.at("importance").get_to(p.level);
        j.at("timestamp").get_to(p.timestamp);
        j.at("hash").get_to(p.hash);
    }

    void EventLogger::SaveLog(const std::vector<AnomalyEntry> &data) {
        std::filesystem::path file{R"(logs\juniper\log.json)"};
        std::ifstream infile(file);
        json j;

        if (infile.good()) {
            infile >> j;
            infile.close();
        } else {
            j["anomalies"] = json::array();
        }

        for (const auto &entry: data) {
            j["anomalies"].emplace_back(entry);
        }

        std::ofstream outfile(file);
        outfile << j.dump(4);
        outfile.close();
    }
} // iotguard