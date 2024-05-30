#ifndef IOTGUARD_EVENTLOGGER_H
#define IOTGUARD_EVENTLOGGER_H

#include "ObserverManager.h"

namespace iotguard {

    ///Event Logger ведет запись всех обнаруженных аномалий и действий приложения.

    class EventLogger : public IObserver {
    public:
        EventLogger() {
            ObserverManager::GetInstance().AddObserver(this);
        }

        ~EventLogger() override {
            ObserverManager::GetInstance().RemoveObserver(this);
        }

        void update(const std::vector<AnomalyEntry> &anomalies) override {
            SaveLog(anomalies);
        }

    private:
        void SaveLog(const std::vector<AnomalyEntry> &data);

    };

} // iotguard

#endif //IOTGUARD_EVENTLOGGER_H
