#pragma once
#include <string>
#include "../app/AppRegistry.hpp"
#include "../app/App.hpp"
#include "../Context.hpp"

inline std::string saveSystem(Context& ctx, AppRegistry& apps) {
    std::ostringstream out;
    out << ctx.kernel.state().serialize();

    for (const auto& [name, app] : apps.all()) {
        if (auto* s = app->serializable()) {
            out << "\n[app." << name << "]\n";
            out << s->saveState() << "\n";
        }
    }
    return out.str();
}

inline void loadSystem(Context& ctx,
                AppRegistry& apps,
                const std::string& data) {
    ctx.kernel.state() = SystemState::deserialize(data);

    std::istringstream in(data);
    std::string line;
    App* currentApp = nullptr;

    while (std::getline(in, line)) {
        if (line.starts_with("[app.")) {
            auto name = line.substr(5, line.size() - 6);
            currentApp = apps.get(name);
            continue;
        }

        if (currentApp) {
            if (auto* s = currentApp->serializable()) {
                s->loadState(line);
            }
        }
    }
}