#include "SystemState.hpp"
#include <sstream>

std::string SystemState::serialize() const {
    std::ostringstream out;
    out << "running=" << running << "\n";
    out << "user.name=" << currentUser.name << "\n";
    out << "user.admin=" << currentUser.isAdmin << "\n";
    out << "devicename=" << deviceName << "\n";
    out << "activeApp=" << activeApp << "\n";
    out << "pwd=" << pwd << "\n";
    out << "version=0.1\n";
    return out.str();
}

SystemState SystemState::deserialize(const std::string& text) {
    SystemState state;
    std::istringstream in(text);
    std::string line;

    auto tobool = [] (std::string str) { return (str == "1" || str == "true"); };

    while (std::getline(in, line)) {
        auto pos = line.find('=');
        if (pos == std::string::npos) continue;

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos+1);

        if (key == "running") {
            state.running = tobool(value);
        } else if (key == "user.name") {
            state.currentUser.name = value;
        } else if (key == "user.admin") {
            state.currentUser.isAdmin = tobool(value);
        } else if (key == "deviceName") {
            state.deviceName = value;
        } else if (key == "activeApp") {
            state.activeApp = value;
        } else if (key == "pwd") {
            state.pwd = value;
        }
    }

    return state;
}
