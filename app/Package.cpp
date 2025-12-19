#include "Package.hpp"
#include "../shell/Shell.hpp"

bool PackageRegistry::install(std::unique_ptr<Package> pkg, Context& ctx, Shell& shell) {
    auto result = m_packages.emplace(pkg.get()->name(), std::move(pkg));
    if (result.second) {
        std::vector<std::unique_ptr<Command>> cmds = pkg.get()->commands();
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