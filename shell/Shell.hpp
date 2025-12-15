#pragma once
#include <filesystem>
#include <cstdlib>
#include "CommandRegistry.hpp"

class Shell {
public:
    Shell(CommandMap commands, IO& io, Context& ctx);
    void run();
    void writePrompt();
    void log(Priority pri, const std::string& message);

private:
    CommandMap m_commands;
    IO& m_io;
    Context& m_ctx;
};
