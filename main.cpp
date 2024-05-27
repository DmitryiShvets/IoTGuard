#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <fstream>
// for convenience
using json = nlohmann::json;
#include "src/DataCollector.h"
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
//    juniper["process.xml"] = { {"url", "rpc/get-system-process-information"},{"headers", {{"Accept", "application/xml"},{"Content-Type", "application/xml"}}}, {"body", "<detail/>"} };
//    juniper["httpd.xml"] = { {"url", "rpc/get-log"},{"headers", {{"Accept", "application/xml"},{"Content-Type", "application/xml"}}}, {"body", "<filename>httpd.log</filename>"} };
//    std::ofstream f1("configs\\juniper_url.json");
//    f1 << juniper;
//    f1.close();

    iotguard::DataCollector collector{"configs\\config.json"};

    collector.GetData();

    return 0;
}
