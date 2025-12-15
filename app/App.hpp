#pragma once
#include <string>
#include <unordered_map>
#include <memory>

class Context;
class SerializableApp {
public:
    virtual ~SerializableApp() = default;

    // Entire block, not line-by-line
    virtual std::string saveState() const = 0;
    virtual void loadState(const std::string& block) = 0;
};

class App {
public:
    virtual ~App() = default;
    virtual std::string name() const = 0;
    virtual void start(Context& ctx) = 0;

    virtual SerializableApp* serializable() { return nullptr; }
};

using AppMap = std::unordered_map<std::string, std::unique_ptr<App>>;
