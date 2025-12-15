#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "../Context.hpp"

class App {
public:
    virtual ~App() = default;
    virtual std::string name() const = 0;
    virtual void start(Context& ctx) = 0;
};

using AppMap = std::unordered_map<std::string, std::unique_ptr<App>>;
