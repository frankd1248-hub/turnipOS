#pragma once
#include <string>

struct User {
    std::string name;
    bool isAdmin = false;
};

struct SystemState {
    bool running = true;
    User currentUser;
    std::string deviceName;
    std::string activeApp;
    std::string pwd;

    std::string serialize() const;
    static SystemState deserialize(const std::string& text);
};
