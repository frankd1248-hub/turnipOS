#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

inline std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

template<typename... T>
inline std::string concat(const std::string& arg, T... others) {
    return arg.append(concat(others));
}

template<typename T>
inline bool contains(std::vector<T> vect, T val) {
    for (T elem : vect) {
        if (elem == val) return true;
    }
    return false;
}