cmake_minimum_required(VERSION 3.28)
project(IoTGuard)

set(CMAKE_CXX_STANDARD 20)

add_executable(IoTGuard main.cpp
        src/DataCollector.cpp
        src/DataCollector.h
        src/DataParser.cpp
        src/DataParser.h
        src/StateComparator.cpp
        src/StateComparator.h
        src/AnomalyDetector.cpp
        src/AnomalyDetector.h
        src/NotificationSystem.cpp
        src/NotificationSystem.h
        src/EventLogger.cpp
        src/EventLogger.h)
