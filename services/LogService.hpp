#pragma once
#include <string>
#include <map>

enum Priority {
    INFO,
    ERROR,
    FATAL
};

const std::map<Priority, std::string> PriorityMap = {
    {Priority::INFO, "[INFO] "},
    {Priority::ERROR, "[ERROR]"},
    {Priority::FATAL, "[FATAL]"}
};

class LogService {
public:
    virtual ~LogService() = default;
    virtual void log(Priority pri, const std::string& message) = 0;
};