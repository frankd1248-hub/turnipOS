#pragma once
#include <unordered_map>
#include <memory>
#include <string>

class App;

class AppRegistry {
public:
    void registerApp(std::unique_ptr<App> app);

    App* get(const std::string& name);
    const auto& all() const { return m_apps; }

private:
    std::unordered_map<std::string, std::unique_ptr<App>> m_apps;
};
