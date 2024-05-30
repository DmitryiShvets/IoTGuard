#ifndef IOTGUARD_ANOMALYDETECTOR_H
#define IOTGUARD_ANOMALYDETECTOR_H

#include <vector>
#include "Stuff.h"
#include "ObserverManager.h"

namespace iotguard {

    ///Anomaly Detector анализирует результаты сравнения, выявляет аномалии и передает их в Notification System и Event Logger.

    class AnomalyDetector {
    public:
        AnomalyDetector();

        void AddRule(DetectionRule rule);

        std::vector<AnomalyEntry> DetectAnomalies(const std::vector<LogEntry> &logs);

    private:
        ObserverManager *observer_manager;
        std::vector<DetectionRule> ruleset;

    };

} // iotguard

#endif //IOTGUARD_ANOMALYDETECTOR_H
