#ifndef IOTGUARD_NOTIFICATIONSYSTEM_H
#define IOTGUARD_NOTIFICATIONSYSTEM_H

#include <iostream>
#include "ObserverManager.h"
#include "Stuff.h"

namespace iotguard {

    ///Notification System уведомляет пользователей о выявленных аномалиях через консольный вывод.

    class NotificationSystem : public IObserver {
    public:
        NotificationSystem() {
            ObserverManager::GetInstance().AddObserver(this);
        }

        ~NotificationSystem() override {
            ObserverManager::GetInstance().RemoveObserver(this);
        }

        void update(const std::vector<AnomalyEntry> &anomalies) override {
            notify(anomalies);
        }

    private:
        void notify(const std::vector<AnomalyEntry> &anomalies) const;

    };
} // iotguard

#endif //IOTGUARD_NOTIFICATIONSYSTEM_H
