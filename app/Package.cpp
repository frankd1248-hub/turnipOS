#include "Package.hpp"
#include "../shell/Shell.hpp"
#include "../io/IO.hpp"
#include "../Context.hpp"

bool PackageRegistry::install(std::unique_ptr<Package> pkg, Context& ctx, Shell& shell) {
    std::string name = pkg.get()->name();
    auto result = m_packages.emplace(name, std::move(pkg));
    if (result.second) {
        std::vector<std::unique_ptr<Command>> cmds = m_packages.at(name).get()->commands();
        for (int i = 0; i < cmds.size(); i++) {
            if (!cmds.at(i)) {
                ctx.io.writeLine("Duplicate installation");
                continue;
            }
            shell.commands().emplace(cmds.at(i).get()->name(), std::move(cmds.at(i)));
        }
    }
    return result.second;
}