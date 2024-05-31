#include <fstream>

#include "StateComparator.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace iotguard {

    std::unordered_set<LogEntry> BaseComparator::load_references(const std::string &filepath) {
        std::ifstream file(filepath);
        if (!file.good()) return {};

        json ref_json;
        file >> ref_json;
        file.close();
        std::unordered_set<LogEntry> references;
        for (const auto &entry: ref_json) {

            references.emplace(LogEntry::from_json(entry));
        }
        return references;
    }

    std::vector<LogEntry> BaseComparator::Compare(const std::vector<LogEntry> &current_data) {
        std::vector<LogEntry> unrecognized_data;
        for (const auto &entry: current_data) {
            if (references.find(entry) == references.end()) {
                unrecognized_data.push_back(entry);
            }
        }
        return unrecognized_data;
    }

    void HttpdComparator::Init() {
        references = load_references(R"(reference\juniper\httpd.json)");
    }

    void ProcessComparator::Init() {
        references = load_references(R"(reference\juniper\process.json)");
    }

    std::vector<LogEntry> StateComparator::Compare(const std::vector<LogEntry> &current_data) {
        if (!comparator) {
            return {};
        }
        return comparator->Compare(current_data);
    }


} // iotguard