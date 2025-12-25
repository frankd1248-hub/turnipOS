#include "./kernel/Kernel.hpp"
#include "./services/Services.hpp"
#include "./services/SimpleAuthService.hpp"
#include "./services/FileLogService.hpp"
#include "./services/LocalFileService.hpp"
#include "./services/SimpleWebService.hpp"
#include "./shell/Shell.hpp"
#include "./commands/Command.hpp"
#include "./io/ConsoleIO.hpp"
#include "./app/Package.hpp"
#include "./app/PersistenceManager.hpp"
#include "./builtin/EditorPackage.hpp"
#include <functional>

void initCommandMap(CommandMap& commands, PackageRegistry& reg, PersistenceManager& man) {
    commands["exit"]    = std::make_unique<ExitCommand>();
    commands["login"]   = std::make_unique<LoginCommand>();
    commands["savepkg"] = std::make_unique<SavePackageCommand>(man, reg);
    commands["loadpkg"] = std::make_unique<LoadPackageCommand>(man, reg);
    commands["pwd"]     = std::make_unique<PWDCommand>();
    commands["chdir"]   = std::make_unique<ChdirCommand>();
    commands["list"]    = std::make_unique<ListCommand>();
    commands["webget"]  = std::make_unique<WebgetCommand>();
    commands["mkdir"]   = std::make_unique<MkdirCommand>();
    commands["touch"]   = std::make_unique<TouchCommand>();
    commands["save"]    = std::make_unique<SaveCommand>();
    commands["load"]    = std::make_unique<LoadCommand>();
    commands["cat"]     = std::make_unique<CatCommand>();
}

int main(int argc, char** argv) {
    CommandMap commands;
    PackageRegistry reg;
    PersistenceManager man;
    initCommandMap(commands, reg, man);

    SystemState state;
    state.deviceName = "turnipOS";
    state.pwd = std::string(std::getenv("HOME")).append("/turnipOS");
    try {
        std::filesystem::current_path(state.pwd);
    } catch (std::filesystem::filesystem_error fe) { 
        std::filesystem::current_path("/");
    }
    Kernel kernel(state);

    Services services;
    services.auth = std::make_unique<SimpleAuthService>();
    services.log = std::make_unique<FileLogService>();
    services.files = std::make_unique<LocalFileService>();
    services.web = std::make_unique<SimpleWebService>();

    ConsoleIO io;
    Context ctx{kernel, services, io, reg};
    Shell shell(std::move(commands), io, ctx);

    std::function<void(Command*)> instCommands = [&shell] (Command* cmd) {
        shell.commands()[cmd->name()] = std::unique_ptr<Command>(cmd);
    };

    reg.install(std::make_unique<EditorPackage>(), ctx, shell);

    shell.run();
}
