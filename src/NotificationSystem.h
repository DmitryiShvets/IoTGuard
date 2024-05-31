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
            add_to_buffer(anomalies);
        }
        void Notify() ;

    private:
        void add_to_buffer(const std::vector<AnomalyEntry> &anomalies);
        std::ostringstream buffer;
    };
} // iotguard

#endif //IOTGUARD_NOTIFICATIONSYSTEM_H
