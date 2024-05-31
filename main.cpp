#include <iostream>
#include <thread>
#include "src/MonitoringTask.h"

int main() {
//    json conf;
//    conf["host"] = "http://192.168.0.29:3030/";
//    conf["device"] = "juniper";
//    conf["learning"] = true;
//    conf["use_cache"] = false;
//    conf["requests-config"] = "configs\\juniper_url.json";
//    conf["user"] = { {"login", "root"}, {"pass", "admin1"} };
//    std::ofstream f("configs\\config.json");
//    f << conf.dump(4);
//    f.close();

//    json juniper;
//    juniper["process"] = { {"url", "rpc/get-system-process-information"},{"headers", {{"Accept", "application/xml"},{"Content-Type", "application/xml"}}}, {"body", "<detail/>"} };
//    juniper["httpd"] = { {"url", "rpc/get-log"},{"headers", {{"Accept", "application/xml"},{"Content-Type", "application/xml"}}}, {"body", "<filename>httpd.log</filename>"} };
//    std::ofstream f1("configs\\juniper_url.json");
//    f1 << juniper;
//    f1.close();

    iotguard::MonitoringTask monitoringTask;
    std::thread monitoringThread(&iotguard::MonitoringTask::Run, &monitoringTask);
    bool is_monitoring = false;
    std::string command;
    while (true) {
        std::cout << "Enter command (start/stop, exit, print, info): ";

        std::getline(std::cin, command);

        if (command == "start") {
            if (!is_monitoring) {
                monitoringTask.Start();
                std::cout << "OK: Monitoring start. \n";
                is_monitoring = true;
            } else {
                std::cout << "ERROR: Monitoring is already started. \n";
            }
        } else if (command == "stop") {
            if (is_monitoring) {
                monitoringTask.Stop();
                std::cout << "OK: Monitoring stop. \n";
                is_monitoring = false;
            } else {
                std::cout << "ERROR: Monitoring is already stopped. \n";
            }
        } else if (command == "exit") {
            monitoringTask.Stop();
            std::cout << "Exit: " << std::endl;
            break;
        } else if (command == "print") {
            std::cout << "Print notifies: \n";
            monitoringTask.Print();
        }
    }

    if (monitoringThread.joinable()) {
        monitoringThread.join();
    }

    return 0;
}

