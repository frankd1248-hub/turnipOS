#pragma once
#include "Command.hpp"

class LoginCommand : public Command {

public:

    LoginCommand() {}

    std::string name() const override { return "login"; }
    std::string help() const override {
        return R"(
        Format: login [USERNAME] [PASSWORD]
        )";
    }

    void execute(Context&, const std::vector<std::string>& args);
};