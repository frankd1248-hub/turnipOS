#include "Shell.hpp"
#include "../Utils.hpp"

Shell::Shell(CommandMap commands, IO& io, Context& ctx) : 
    m_commands(std::move(commands)), m_io(io), m_ctx(ctx) { }

void Shell::run() {
    std::string command;
    std::vector<std::string> args;
    while (m_ctx.kernel.state().running) {
        writePrompt();

        command = m_io.readLine();
        args = splitString(command, ' ');
        if (args.size() == 0) {
            continue;
        }

        command = args.at(0);
        args.erase(args.begin());
        if (m_ctx.kernel.state().currentUser.name == "") {
            if (command != "login") {
                m_io.writeLine("Cannot execute commands other than 'login' when not logged in");
                log(INFO, concat(
                    "User 'login' attempted to use command '",
                    command,
                    "'"
                ));
                continue;
            }
        }

        m_commands.at(command).get()->execute(m_ctx, args);
    }
    return;
}

void Shell::writePrompt() {
    m_io.write(
        concat(m_ctx.kernel.state().currentUser.name != "" ? 
               std::string(m_ctx.kernel.state().currentUser.name) : std::string("login")
               ,"@"
               ,m_ctx.kernel.state().deviceName
               ,"> ")
    );
}

void Shell::log(Priority pri, const std::string& message) {
    m_ctx.services.log.get()->log(pri, message);
}