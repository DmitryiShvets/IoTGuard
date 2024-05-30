#include "NotificationSystem.h"

namespace iotguard {
    void NotificationSystem::notify(const std::vector<AnomalyEntry> &anomalies) const {
        for (const auto &anomaly: anomalies){
            std::cout << "Timestamp: " << anomaly.timestamp << "\n";
            std::cout << "Importance: " <<LEVEL[anomaly.level] << "\n";
            std::cout << "Type: " << anomaly.type << "\n";
            std::cout << "Data: " << anomaly.data << "\n";
            std::cout << "--------------------------------------\n";
        }
    }
} // iotguard