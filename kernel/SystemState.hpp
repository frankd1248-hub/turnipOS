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
};