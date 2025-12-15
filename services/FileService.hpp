#pragma once
#include <string>

class FileService {
public:
    virtual ~FileService() = default;
    virtual void write(const std::string& path, const std::string& data) = 0;
    virtual std::string read(const std::string& path) = 0;
};
