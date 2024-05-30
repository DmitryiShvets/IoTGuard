cmake_minimum_required(VERSION 3.28)
project(IoTGuard)

set(CMAKE_CXX_STANDARD 20)

set(DEFAULT_BUILD_TYPE "Release")
if (EXISTS "${CMAKE_SOURCE_DIR}/.git")
    set(DEFAULT_BUILD_TYPE "Debug")
endif ()

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif ()
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
        src/EventLogger.h
        src/ObserverManager.cpp
        src/ObserverManager.h
        src/Stuff.h)

if (WIN32) # Install dlls in the same directory as the executable on Windows
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/)
endif ()

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git GIT_TAG 3020c34ae2b732121f37433e61599c34535e68a8)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(pugixml GIT_REPOSITORY https://github.com/zeux/pugixml.git)
FetchContent_MakeAvailable(pugixml)

target_link_libraries(IoTGuard PRIVATE cpr::cpr nlohmann_json::nlohmann_json pugixml::pugixml)

set_target_properties(IoTGuard PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/)
add_custom_command(TARGET IoTGuard POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/configs $<TARGET_FILE_DIR:IoTGuard>/configs)
