#include "ObserverManager.h"

namespace iotguard {
    ObserverManager &ObserverManager::GetInstance() {
        static ObserverManager instance;
        return instance;
    }

    void ObserverManager::AddObserver(IObserver *observer) {
        observers.push_back(observer);
    }

    void ObserverManager::RemoveObserver(IObserver *observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void ObserverManager::NotifyObservers(const std::vector<AnomalyEntry> &anomalies) {
        for (auto *observer: observers) {
            observer->update(anomalies);
        }
    }

} // iotguard

