#pragma once
#include <string>

class IO {
public:
    virtual ~IO() = default;
    virtual std::string readLine() = 0;
    virtual void writeLine(const std::string&) = 0;
    virtual void write(const std::string&) = 0;
};