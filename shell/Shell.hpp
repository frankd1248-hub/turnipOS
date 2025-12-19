#pragma once
#include <filesystem>
#include <cstdlib>
#include "../commands/Command.hpp"
#include "../services/LogService.hpp"
#include "../kernel/Kernel.hpp"
#include "../io/IO.hpp"
#include "../services/Services.hpp"

class Shell {
public:
    Shell(CommandMap commands, IO& io, Context& ctx);
    void run();
    void writePrompt();
    void log(Priority pri, const std::string& message);

    CommandMap& commands() { return m_commands; }

private:
    CommandMap m_commands;
    IO& m_io;
    Context& m_ctx;
};
