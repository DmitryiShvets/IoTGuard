//
// Created by Dmitry on 30.05.2024.
//

#include "MonitoringTask.h"


namespace iotguard {
    MonitoringTask::MonitoringTask()
            : stopFlag(false), running(false) {
        std::ifstream file(config_file);
        json config = json::parse(file, nullptr, false);
        file.close();
        if (config.is_discarded()) {
            throw std::invalid_argument(std::format("file {} parse error", config_file));
        }

        httpdParser = std::make_unique<iotguard::HttpdParser>(config["learning"], config["use_cache"]);
        processParser = std::make_unique<iotguard::ProcessParser>(config["learning"]);
        httpdComparator = std::make_unique<iotguard::HttpdComparator>();
        processComparator = std::make_unique<iotguard::ProcessComparator>();

        httpdComparator->Init();
        processComparator->Init();

        anomalyDetector.AddRule({"", std::regex("(.*)"), iotguard::IMPORTANCE_LEVEL::INFO, 0});
        anomalyDetector.AddRule(
                {"", std::regex(".*(/?PHPRC=/dev/fd/0).*"), iotguard::IMPORTANCE_LEVEL::CRITICAL, 0});
    }

    void MonitoringTask::Start() {
        std::unique_lock<std::mutex> lock(mtx);
        stopFlag = false;
        running = true;
        cv.notify_all();
    }

    void MonitoringTask::Stop() {
        std::unique_lock<std::mutex> lock(mtx);
        stopFlag = true;
        cv.notify_all();
    }

    void MonitoringTask::Run() {
        std::vector<LogEntry> unrecognized_data;
        std::vector<AnomalyEntry> anomaly_data;

        while (true) {
            {
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, [this] { return running || stopFlag; });
                if (stopFlag) {
                    running = false;
                    break;
                }
            }
            if (!collector.GetData()) {
                std::cerr << "failed to fetch data from remote device" << std::endl;
                break;
            }

            dataParser.SetParser(httpdParser.get());
            comparator.SetComparator(httpdComparator.get());

            for (const auto &entry: fs::directory_iterator(R"(logs\juniper\httpd)")) {
                if (entry.is_regular_file()) {
                    const auto &filePath = entry.path().string();

                    auto parse_result = dataParser.Parse(filePath);
                    unrecognized_data = comparator.Compare(parse_result);
                    anomaly_data = anomalyDetector.DetectAnomalies(unrecognized_data);
                    fs::remove(entry.path());
                }
            }

            dataParser.SetParser(processParser.get());
            comparator.SetComparator(processComparator.get());

            for (const auto &entry: fs::directory_iterator(R"(logs\juniper\process)")) {
                if (entry.is_regular_file()) {
                    const auto &filePath = entry.path().string();

                    auto parse_result = dataParser.Parse(filePath);
                    unrecognized_data = comparator.Compare(parse_result);
                    anomaly_data = anomalyDetector.DetectAnomalies(unrecognized_data);
                    fs::remove(entry.path());
                }
            }

            std::this_thread::sleep_for(std::chrono::seconds(15));
        }
    }

    void MonitoringTask::Print() {
        notifier.Notify();
    }
} // iotguard