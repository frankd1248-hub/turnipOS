#pragma once
#include <optional>
#include <functional>
#include "Command.hpp"
#include "../Schedule.hpp"

class ExitCommand : public Command {
public:

    ExitCommand() {}

    std::string name() const override {return "exit";}
    std::string help() const override {return "exit the shell";}

    void execute(Context&, const std::vector<std::string>& args) override;

private:
};