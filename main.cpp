#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <fstream>
// for convenience
using json = nlohmann::json;


#include "src/DataCollector.h"
#include "src/DataParser.h"
#include "src/StateComparator.h"
#include "src/AnomalyDetector.h"
#include "src/NotificationSystem.h"
#include "src/EventLogger.h"
int main() {

//    json conf;
//    conf["host"] = "http://192.168.0.29:3030/";
//    conf["device"] = "juniper";
//    conf["requests-config"] = "configs\\juniper_url.json";
//    conf["user"] = { {"login", "root"}, {"pass", "admin1"} };
//    std::ofstream f("configs\\config.json");
//    f << conf;
//    f.close();

//    json juniper;
//    juniper["process"] = { {"url", "rpc/get-system-process-information"},{"headers", {{"Accept", "application/xml"},{"Content-Type", "application/xml"}}}, {"body", "<detail/>"} };
//    juniper["httpd"] = { {"url", "rpc/get-log"},{"headers", {{"Accept", "application/xml"},{"Content-Type", "application/xml"}}}, {"body", "<filename>httpd.log</filename>"} };
//    std::ofstream f1("configs\\juniper_url.json");
//    f1 << juniper;
//    f1.close();

    iotguard::DataCollector collector{"configs\\config.json"};
    iotguard::DataParser dataParser;
    std::unique_ptr<iotguard::HttpdParser> httpdParser = std::make_unique<iotguard::HttpdParser>(false);
    std::unique_ptr<iotguard::ProcessParser> processParser = std::make_unique<iotguard::ProcessParser>();
    std::unique_ptr<iotguard::HttpdComparator> httpdComparator = std::make_unique<iotguard::HttpdComparator>();
    std::unique_ptr<iotguard::ProcessComparator> processComparator = std::make_unique<iotguard::ProcessComparator>();
    iotguard::NotificationSystem notifier;
    iotguard::EventLogger logger;
    
    httpdComparator->Init();
    processComparator->Init();
    iotguard::StateComparator comparator;

    //collector.GetData();
    dataParser.SetParser(httpdParser.get());
    //dataParser.SetParser(processParser.get());
    auto parse_result = dataParser.Parse(R"(logs\juniper\httpd\1716905013.xml)");
    //auto parse_result = dataParser.Parse(R"(logs\juniper\process\1716905013.xml)");
    comparator.SetComparator(httpdComparator.get());
    //comparator.SetComparator(processComparator.get());
    auto unrecognized_data = comparator.Compare(parse_result);

    iotguard::AnomalyDetector anomalyDetector;
    anomalyDetector.AddRule({"", std::regex("(.*)"), iotguard::IMPORTANCE_LEVEL::INFO, 0});
    anomalyDetector.AddRule({"", std::regex(".*(/?PHPRC=/dev/fd/0).*"), iotguard::IMPORTANCE_LEVEL::CRITICAL, 0});
    auto anomaly_data = anomalyDetector.DetectAnomalies(unrecognized_data);


    return 0;
}
