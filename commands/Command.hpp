#pragma once
#include <string>
#include <vector>
#include "../Context.hpp"
#include "../shell/Shell.hpp"

class Command {
public:
    virtual ~Command() = default;
    virtual std::string name() const = 0;
    virtual std::string help() const = 0;
    virtual void execute(Context&, Shell&, const std::vector<std::string>& args) = 0;
};

class ExitCommand : public Command {
public:

    ExitCommand() {}

    std::string name() const override {return "exit";}
    std::string help() const override {return "exit the shell";}

    void execute(Context& ctx, Shell& sh, const std::vector<std::string>& args) override {
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

    void execute(Context& ctx, Shell& sh, const std::vector<std::string>& args) override {
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

    void execute(Context& ctx, Shell& sh, const std::vector<std::string>& args) override {
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

    void execute(Context& ctx, Shell& sh,
                 const std::vector<std::string>& args) override {
        if (args.empty()) {
            ctx.io.writeLine("Usage: load <file>");
            return;
        }

        std::string data = ctx.services.files->read(args[0]);
        SystemState loaded = SystemState::deserialize(data);

        ctx.kernel.setState(loaded);
        ctx.io.writeLine("System state loaded.");
    }
};

class PWDCommand : public Command {
public:
    std::string name() const override { return "pwd"; }
    std::string help() const override { return "pwd"; }

    void execute(Context& ctx, Shell& sh, const std::vector<std::string>& args) {
        ctx.io.writeLine(sh.getPWD());
    }
};