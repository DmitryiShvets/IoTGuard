//
// Created by Dmitry on 30.05.2024.
//

#ifndef IOTGUARD_OBSERVERMANAGER_H
#define IOTGUARD_OBSERVERMANAGER_H


#include <vector>
#include "Stuff.h"

namespace iotguard {
    class IObserver {
    public:
        virtual ~IObserver() = default;

        virtual void update(const std::vector<AnomalyEntry> &anomalies) = 0;
    };

    class ObserverManager {
    public:
        static ObserverManager &GetInstance();

        void AddObserver(IObserver *observer);

        void RemoveObserver(IObserver *observer);

        void NotifyObservers(const std::vector<AnomalyEntry> &anomalies);

    private:
        ObserverManager() = default;

        std::vector<IObserver *> observers;
    };
}

#endif //IOTGUARD_OBSERVERMANAGER_H
