#include "ConsoleIO.hpp"

std::string ConsoleIO::readLine() {
    std::string line;
    std::getline(std::cin, line);
    return line;
}

void ConsoleIO::writeLine(const std::string& line) {
    std::cout << line << "\n";
}

void ConsoleIO::write(const std::string& line) {
    std::cout << line;
}