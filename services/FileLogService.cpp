#include "FileLogService.hpp"

FileLogService::FileLogService() {
    output.open(defaultLogPath.c_str());
    if (!output.is_open()) {
        std::cerr << "Could not open log file, aborting";
    }
}

FileLogService::~FileLogService() {
    output.close();
}

void FileLogService::log(Priority pri, const std::string& message) {
    output << PriorityMap.at(pri) << " " << message << "\n";
}