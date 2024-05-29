#include "AnomalyDetector.h"

namespace iotguard {
    std::vector<AnomalyEntry> AnomalyDetector::DetectAnomalies(const std::vector<LogEntry> &logs) {
        std::vector<AnomalyEntry> anomalies;
        std::unordered_map<size_t, size_t> event_count;

        // Проверяем правила
        for (const auto &rule: ruleset) {
            for (const auto &entry: logs) {
                bool isAnomaly = false;

                if ((entry.type == rule.entry_type) || (rule.entry_type.empty())) {
                    if (std::regex_match(entry.data, rule.signature)) {
                        isAnomaly = true;
                        event_count[entry.hash]++;
                    }

                }

                if (isAnomaly && event_count[entry.hash] > rule.threshold) {
                    anomalies.push_back({entry.type, entry.data, rule.level, entry.timestamp, entry.hash});
                }
            }
        }

        return anomalies;
    }

    void AnomalyDetector::AddRule(DetectionRule rule) {
        ruleset.push_back(std::move(rule));
    }

    std::ostream &operator<<(std::ostream &os, const AnomalyEntry &entry) {
        os <<"anomaly detected... level: " << LEVEL[entry.level] << " type: " << entry.type << " data: " << entry.data << " timestamp: "
           << entry.timestamp << " hash: " << entry.hash;
        return os;
    }
} // iotguard