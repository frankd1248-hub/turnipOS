#include "Shell.hpp"
#include "../Utils.hpp"

Shell::Shell(CommandMap commands, IO& io, Context& ctx) : 
    m_commands(std::move(commands)), m_io(io), m_ctx(ctx) { }

void Shell::run() {
    std::string command;
    std::vector<std::string> args;
    m_ctx.kernel.state().activeApp = "tsh";
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
            if (command != "login" && command != "exit") {
                m_io.writeLine("Cannot execute commands other than 'login' and 'exit' when not logged in");
                log(INFO, concat(
                    "User 'login' attempted to use command '",
                    command,
                    "'"
                ));
                continue;
            }
        }

        if (m_commands.count(command) == 0) {
            m_io.writeLine(concat("Unknown command '", command, "'"));
            continue;
        }

        m_commands.at(command).get()->execute(m_ctx, args);
    }
    return;
}

void Shell::writePrompt() {
    m_io.write(
        concat("[",
               m_ctx.kernel.state().currentUser.name != "" ?
                    std::string(m_ctx.kernel.state().currentUser.name) : std::string("login")
               ,"@"
               ,m_ctx.kernel.state().deviceName,
               " ",
               getPWD(),
               "]$ ")
    );
}

std::string Shell::getPWD() const {
    std::string path = std::filesystem::canonical(std::filesystem::current_path()).string();
    path = replaceHomeWithTilde(path);
    return path;
}

std::string Shell::replaceHomeWithTilde(const std::string& path) const {
    const char* homeEnv = std::getenv("HOME");
    if (!homeEnv)
        return path;

    std::string home(homeEnv);

    // Remove trailing slash from HOME (except "/")
    if (home.size() > 1 && home.back() == '/')
        home.pop_back();

    // Exact match
    if (path == home)
        return "~";

    // Prefix match
    if (path.size() > home.size() &&
        path.compare(0, home.size(), home) == 0 &&
        path[home.size()] == '/')
    {
        return "~" + path.substr(home.size());
    }

    return path;
}

void Shell::log(Priority pri, const std::string& message) {
    m_ctx.services.log.get()->log(pri, message);
}
