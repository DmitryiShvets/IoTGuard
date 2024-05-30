#ifndef IOTGUARD_STATECOMPARATOR_H
#define IOTGUARD_STATECOMPARATOR_H

#include <unordered_set>
#include "Stuff.h"

namespace iotguard {

    ///State Comparator сравнивает текущее состояние системы с эталонным состоянием и передает результаты в Anomaly Detector.

    class BaseComparator {
    public:
        std::unordered_set<LogEntry> references;

        static std::unordered_set<LogEntry> load_references(const std::string &filepath);

        virtual ~BaseComparator() = default;

        virtual void Init() = 0;

        std::vector<LogEntry> Compare(const std::vector<LogEntry> &current_data);

    };

    class HttpdComparator : public BaseComparator {
    public:
        void Init() override;
    };

    class ProcessComparator : public BaseComparator{
     public:
        void Init() override;
    };

    class StateComparator {
        BaseComparator *comparator = nullptr;

    public:
        void SetComparator(BaseComparator *p) {
            this->comparator = p;
        }

        std::vector<LogEntry> Compare(const std::vector<LogEntry> &current_data);
    };

} // iotguard

#endif //IOTGUARD_STATECOMPARATOR_H
