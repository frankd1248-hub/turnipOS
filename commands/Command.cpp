#include "Command.hpp"
#include "../Context.hpp"
#include "../kernel/Kernel.hpp"
#include "../io/IO.hpp"
#include "../app/Package.hpp"
#include "../app/PersistenceManager.hpp"
#include "../services/Services.hpp"

void ExitCommand::execute(Context& ctx, const std::vector<std::string>& args) {
    ctx.kernel.shutdown();
};

void LoginCommand::execute(Context& ctx, const std::vector<std::string>& args) {
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

void SaveCommand::execute(Context& ctx, const std::vector<std::string>& args) {
    try {
        if (args.size() == 1) {
            // save all
            for (const std::pair<const std::string, std::unique_ptr<Package>>& pair : m_registry.all()) {
                Package& pkg = *pair.second.get();
                m_pm.savePackage(pkg);
            }
            return;
        }
        if (args.size() == 2) {
            Package* pkg = m_registry.get(args[1]);
            if (!pkg) {
                ctx.io.writeLine("Error: Package not found");
                return;
            }
            m_pm.savePackage(*pkg);
            return;
        }
        ctx.io.writeLine("Usage: save [package]");
        return;
    }
    catch (const std::exception& e) {
        ctx.io.writeLine(e.what());
        return;
    }
}

void LoadCommand::execute(Context& ctx, const std::vector<std::string>& args) {
    try {
        if (args.size() == 1) {
            for (const std::pair<const std::string, std::unique_ptr<Package>>& pair : m_registry.all()) {
                Package& pkg = *pair.second.get();
                m_pm.loadPackage(pkg);
            }
            return;
        }

        if (args.size() == 2) {
            Package* pkg = m_registry.get(args[1]);
            if (!pkg) {
                ctx.io.writeLine("Error: Package not found");
                return;
            }
            m_pm.loadPackage(*pkg);
            return;
        }

        ctx.io.writeLine("Usage: load [package]");
        return;
    }
    catch (const std::exception& e) {
        ctx.io.writeLine(e.what());
        return;
    }
}

void PWDCommand::execute(Context& ctx, const std::vector<std::string>& args) {
    ctx.io.writeLine(getPWD());
}

void ChdirCommand::execute(Context& ctx, const std::vector<std::string>& args) {
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

void ListCommand::execute(Context& ctx, const std::vector<std::string>& args) {
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

void WebgetCommand::execute(Context& ctx, const std::vector<std::string>& args) {
    if (args.size() != 2) {
        ctx.io.writeLine("Usage: webget \"<url>\" <path>");
        return;
    }

    std::string url = args.at(0).substr(1, args.at(0).size() - 2);
    std::filesystem::path path{args.at(1)};

    try {
        ctx.services.web.get()->downloadFileToPath(url, path);
        ctx.io.writeLine("Downloaded file");
    } catch (std::runtime_error exc) {
        ctx.io.writeLine(exc.what());
    }
}