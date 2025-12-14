#pragma once
#include "LogService.hpp"
#include <iostream>
#include <fstream>

class FileLogService : public LogService {

public:
    FileLogService();
    ~FileLogService();
    void log(Priority pri, const std::string& message) override;

private:
    const std::string defaultLogPath = "./syslog";
    std::ofstream output = std::ofstream(defaultLogPath);
};