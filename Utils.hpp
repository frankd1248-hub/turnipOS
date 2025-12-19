#pragma once
#include <filesystem>
#include <vector>
#include <string>
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

inline std::string replaceHomeWithTilde(const std::string& path) {
    const char* homeEnv = std::getenv("HOME");
    if (!homeEnv)
        return path;

    std::string home(homeEnv);

    // Remove trailing slash from HOME (except "/")
    if (home.size() > 1 && home.back() == '/')
        home.pop_back();

    // Exact match
    if (path == home)
        return "~";

    // Prefix match
    if (path.size() > home.size() &&
        path.compare(0, home.size(), home) == 0 &&
        path[home.size()] == '/')
    {
        return "~" + path.substr(home.size());
    }

    return path;
}

inline std::string getPWD() {
    std::string path = std::filesystem::canonical(std::filesystem::current_path()).string();
    path = replaceHomeWithTilde(path);
    return path;
}

template<typename... Ts>
inline std::string concat(Ts&&... ts) {
    std::stringstream ss;
    (ss << ... << std::forward<Ts>(ts));
    return ss.str();
}

template<typename T>
inline bool contains(std::vector<T> vect, T val) {
    for (T elem : vect) {
        if (elem == val) return true;
    }
    return false;
}


#ifdef WIN32

#include <windows.h>

inline std::vector<int> getTerminalSize() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    return {rows, columns};
}

#else

#include <sys/ioctl.h>
#include <unistd.h>

inline std::vector<int> getTerminalSize() {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return {size.ws_row, size.ws_col};
}

#endif