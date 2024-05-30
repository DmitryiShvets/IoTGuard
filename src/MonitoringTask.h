//
// Created by Dmitry on 30.05.2024.
//

#ifndef IOTGUARD_MONITORINGTASK_H
#define IOTGUARD_MONITORINGTASK_H

#include "DataCollector.h"
#include "DataParser.h"
#include "StateComparator.h"
#include "AnomalyDetector.h"
#include "NotificationSystem.h"
#include "EventLogger.h"

namespace iotguard {

    class MonitoringTask {
    public:
        MonitoringTask();

        void Start();

        void Stop();

        void Run();

        void Print();

    private:
        std::mutex mtx;
        std::condition_variable cv;
        std::atomic<bool> stopFlag;
        bool running;
        std::string config_file=R"(configs\config.json)";
        iotguard::DataCollector collector{config_file};
        iotguard::DataParser dataParser;
        std::unique_ptr<iotguard::HttpdParser> httpdParser;
        std::unique_ptr<iotguard::ProcessParser> processParser;
        std::unique_ptr<iotguard::HttpdComparator> httpdComparator;
        std::unique_ptr<iotguard::ProcessComparator> processComparator;
        iotguard::StateComparator comparator;
        iotguard::NotificationSystem notifier;
        iotguard::EventLogger logger;
        iotguard::AnomalyDetector anomalyDetector;

    };

} // iotguard

#endif //IOTGUARD_MONITORINGTASK_H
