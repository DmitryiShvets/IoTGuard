#include "NotificationSystem.h"

namespace iotguard {
    void NotificationSystem::Notify()  {
        std::cout << buffer.str() << std::endl;
        buffer.str("");
        buffer.clear();
    }

    void NotificationSystem::add_to_buffer(const std::vector<AnomalyEntry> &anomalies) {
        for (const auto &anomaly: anomalies) {
            buffer << "Timestamp: " << anomaly.timestamp << "\n";
            buffer << "Importance: " << LEVEL[anomaly.level] << "\n";
            buffer << "Type: " << anomaly.type << "\n";
            buffer << "Data: " << anomaly.data << "\n";
            buffer << "--------------------------------------\n";
        }
    }
} // iotguard