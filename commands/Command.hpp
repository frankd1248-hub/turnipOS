#pragma once
#include <string>
#include <vector>
#include "../Utils.hpp"
#include "../app/App.hpp"

class Shell;
class PersistenceManager;
class PackageRegistry;

class Command {
public:
    virtual ~Command() = default;
    virtual std::string name() const = 0;
    virtual std::string help() const = 0;
    virtual void execute(Context&, const std::vector<std::string>& args) = 0;
};

class ExitCommand : public Command {
public:

    ExitCommand() {}
    ~ExitCommand() {}

    std::string name() const override {return "exit";}
    std::string help() const override {return "exit the shell";}

    void execute(Context& ctx, const std::vector<std::string>& args) override;

};

class LoginCommand : public Command {

public:

    LoginCommand() {}
    ~LoginCommand() {}

    std::string name() const override { return "login"; }
    std::string help() const override {
        return R"(
        Format: login [USERNAME] [PASSWORD]
        )";
    }

    void execute(Context& ctx, const std::vector<std::string>& args) override;
};

class SaveCommand : public Command {
public:
    SaveCommand(PersistenceManager& pm, PackageRegistry& registry) : m_pm(pm), m_registry(registry) {}
    ~SaveCommand() {}

    std::string name() const override { return "save"; }
    std::string help() const override { return "Usage: save <file>"; }

    void execute(Context& ctx, const std::vector<std::string>& args) override;

private:
    PersistenceManager& m_pm;
    PackageRegistry& m_registry;
};

class LoadCommand : public Command {
public:
    LoadCommand(PersistenceManager& pm, PackageRegistry& registry) : m_pm(pm), m_registry(registry) {}
    ~LoadCommand() {}

    std::string name() const override { return "load"; }
    std::string help() const override { return "Usage: load <file>"; }

    void execute(Context& ctx, const std::vector<std::string>& args) override;

private:
    PersistenceManager& m_pm;
    PackageRegistry& m_registry;
};

class PWDCommand : public Command {
public:
    PWDCommand() {}
    ~PWDCommand() {}

    std::string name() const override { return "pwd"; }
    std::string help() const override { return "pwd"; }

    void execute(Context& ctx, const std::vector<std::string>& args) override;
};

class ChdirCommand : public Command {
public:
    ChdirCommand() {}
    ~ChdirCommand() {}

    std::string name() const override { return "chdir"; }
    std::string help() const override { return "chdir <path>"; }

    void execute(Context& ctx, const std::vector<std::string>& args) override;
};

class ListCommand : public Command {
public:
    ListCommand() {}
    ~ListCommand() {}

    std::string name() const override { return "list"; }
    std::string help() const override { return "list"; }

    void execute(Context& ctx, const std::vector<std::string>& args) override;
};

class WebgetCommand : public Command {
public:
    WebgetCommand() {}
    ~WebgetCommand() {}

    std::string name() const override { return "webget"; }
    std::string help() const override { return "webget \"<url>\" <path>"; }

    void execute(Context& ctx, const std::vector<std::string>& args) override;
};

class MkdirCommand : public Command {
public:
    MkdirCommand() {}
    ~MkdirCommand() {}

    std::string name() const override { return "mkdir"; }
    std::string help() const override { return "mkdir <directory path>"; }

    void execute(Context& ctx, const std::vector<std::string>& args) override;
};