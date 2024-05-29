#ifndef IOTGUARD_ANOMALYDETECTOR_H
#define IOTGUARD_ANOMALYDETECTOR_H

#include <vector>
#include <regex>
#include <functional>
#include <ostream>
#include "DataParser.h"

namespace iotguard {

    ///Anomaly Detector анализирует результаты сравнения, выявляет аномалии и передает их в Notification System и Event Logger.

    enum IMPORTANCE_LEVEL {
        INFO,      // Информационные сообщения
        UNDEFINED,    // Нераспознанные, требующие внимания
        WARNING,   // Предупреждения
        ERR,     // Ошибки
        CRITICAL,  // Критические ошибки
        THRESHOLD
    };
    constexpr std::string_view LEVEL[] =
            {
                    "INFO",
                    "UNDEFINED",
                    "WARNING",
                    "ERR",
                    "CRITICAL",
                    "THRESHOLD",

            };


    struct DetectionRule {
        std::string entry_type; // Тип аномалии
        std::regex signature;  // Сигнатура аномалии
        IMPORTANCE_LEVEL level; // Уровень важности правила
        int threshold; // Порог срабатывания правила
    };
    struct AnomalyEntry {
        friend std::ostream &operator<<(std::ostream &os, const AnomalyEntry &entry);

        std::string type;  // Тип аномалии
        std::string data;  // Сообщение лога, связанное с аномалией
        IMPORTANCE_LEVEL level; // Уровень важности аномалии
        std::string timestamp; // Время обнаружения аномалии
        size_t hash; // Хеш аномалии
    };

        class AnomalyDetector {
    public:
        void AddRule(DetectionRule rule);

        std::vector<AnomalyEntry> DetectAnomalies(const std::vector<LogEntry> &logs);
    private:
        std::vector<DetectionRule> ruleset;

    };

} // iotguard

#endif //IOTGUARD_ANOMALYDETECTOR_H
