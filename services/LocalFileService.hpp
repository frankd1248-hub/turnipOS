#pragma once
#include <fstream>
#include <sstream>
#include "FileService.hpp"

class LocalFileService : public FileService {
public:
    void write(const std::string& path, const std::string& data) override {
        std::ofstream out(path);
        out << data;
    }

    std::string read(const std::string& path) override {
        std::ifstream in(path);
        std::ostringstream ss;
        ss << in.rdbuf();
        return ss.str();
    }

};
