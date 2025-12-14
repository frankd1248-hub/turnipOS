#pragma once
#include <iostream>
#include "IO.hpp"

/// @brief Console IO
class ConsoleIO : public IO {

    std::string readLine() override;
    void writeLine(const std::string& line) override;
    void write(const std::string& line) override;
};