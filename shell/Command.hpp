#pragma once
#include <string>
#include <vector>
#include "../Context.hpp"

class Command {
public:
    virtual ~Command() = default;
    virtual std::string name() const = 0;
    virtual std::string help() const = 0;
    virtual void execute(Context&, const std::vector<std::string>& args) = 0;
};