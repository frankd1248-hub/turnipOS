#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include "../Context.hpp"
#include "../commands/Command.hpp"
#include "../io/IO.hpp"
#include "App.hpp"

class Shell;

class Package {
public:
    virtual ~Package() = default;

    virtual std::string name() const = 0;
    virtual uint32_t stateVersion() const = 0;

    virtual void install(Context&, Shell&) = 0;
    virtual void uninstall(Context&, Shell&) = 0;

    virtual std::vector<std::unique_ptr<App>> apps() = 0;
    virtual std::vector<std::unique_ptr<Command>> commands() = 0;

    virtual void save(class Serializer& out) const = 0;
    virtual void load(class Deserializer& in) = 0;

    virtual void migrate(uint32_t fromVersion) {}
};


class PackageRegistry {
public:
    bool install(std::unique_ptr<Package> pkg, Context& ctx, Shell& shell);

    void uninstall(const std::string& name, Context&) {
        m_packages.erase(name);
    }

    Package* get(const std::string& name) {
        return m_packages.at(name).get();
    }

    const auto& all() const { return m_packages; }

private:
    std::unordered_map<std::string, std::unique_ptr<Package>> m_packages;
};