#pragma once
#include <string>
#include <vector>
#include "../Context.hpp"
#include "../Utils.hpp"

class Shell;

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

    std::string name() const override {return "exit";}
    std::string help() const override {return "exit the shell";}

    void execute(Context& ctx, const std::vector<std::string>& args) override {
        ctx.kernel.shutdown();
    };

};

class LoginCommand : public Command {

public:

    LoginCommand() {}

    std::string name() const override { return "login"; }
    std::string help() const override {
        return R"(
        Format: login [USERNAME] [PASSWORD]
        )";
    }

    void execute(Context& ctx, const std::vector<std::string>& args) override {
        if (args.size() < 2) {
            ctx.io.writeLine("Error: insufficient arguments for login command");
            return;
        } else {
            if (ctx.services.auth.get()->login(args.at(0), args.at(1))) {
                ctx.kernel.state().currentUser.name = args.at(0);
                ctx.kernel.state().currentUser.isAdmin = (args.at(0) == "root");
                if (args.at(0) == "root") {
                    ctx.io.writeLine("Logged in as root user");
                } else {
                    ctx.io.writeLine(std::string("Logged in as regular user ").append(args.at(0)));
                }
            } else {
                ctx.io.writeLine("Authentication failed");
            }
        }
        return;
    }
};

class SaveCommand : public Command {

public:
    std::string name() const override { return "save"; }
    std::string help() const override { return "Format: save <file>"; }

    void execute(Context& ctx, const std::vector<std::string>& args) override {
        if (args.empty()) {
            ctx.io.writeLine("Usage: save <file>");
            return;
        }

        std::string data = ctx.kernel.state().serialize();
        ctx.services.files->write(args[0], data);
        ctx.io.writeLine("System state saved");
    }
};

class LoadCommand : public Command {
public:
    std::string name() const override { return "load"; }
    std::string help() const override { return "load <file>"; }

    void execute(Context& ctx, const std::vector<std::string>& args) override {
        if (args.empty()) {
            ctx.io.writeLine("Usage: load <file>");
            return;
        }

        std::string data = ctx.services.files->read(args[0]);
        SystemState loaded = SystemState::deserialize(data);

        ctx.kernel.state() = loaded;
        ctx.io.writeLine("System state loaded.");
    }
};

class PWDCommand : public Command {
public:
    std::string name() const override { return "pwd"; }
    std::string help() const override { return "pwd"; }

    void execute(Context& ctx, const std::vector<std::string>& args) {
        ctx.io.writeLine(getPWD());
    }
};

class ChdirCommand : public Command {
public:
    std::string name() const override { return "chdir"; }
    std::string help() const override { return "chdir <path>"; }

    void execute(Context& ctx, const std::vector<std::string>& args) {
        if (args.empty()) {
            ctx.io.writeLine("Usage: chdir <path>");
            return;
        }

        std::string path = args.at(0);
        if (path.at(0) == '~') {
            path.replace(0, 1, std::getenv("HOME"));
        }
        std::filesystem::current_path(std::filesystem::path(path));
    }
};

class ListCommand : public Command {
public:
    std::string name() const override { return "list"; }
    std::string help() const override { return "list"; }

    void execute(Context& ctx, const std::vector<std::string>& args) {
        const std::filesystem::path target{"."};
        try {
            for (auto const& dirEntry : std::filesystem::directory_iterator(target)) {
                std::string name = splitString(dirEntry.path(), '/').back();
                if (name.at(0) != '.')
                    ctx.io.write(splitString(dirEntry.path(), '/').back() + "    ");
            }
            ctx.io.writeLine("");
        } catch (std::filesystem::filesystem_error const& exc) {
            ctx.io.writeLine("Error occurred while looking for files.");
        }
    }
};