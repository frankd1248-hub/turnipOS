#include "./kernel/Kernel.hpp"
#include "./services/Services.hpp"
#include "./services/SimpleAuthService.hpp"
#include "./services/FileLogService.hpp"
#include "./services/LocalFileService.hpp"
#include "./services/SimpleWebService.hpp"
#include "./shell/Shell.hpp"
#include "./shell/CommandRegistry.hpp"
#include "./commands/Command.hpp"
#include "./io/ConsoleIO.hpp"
#include "./app/Package.hpp"
#include "./app/PersistenceManager.hpp"

void initCommandMap(CommandMap& commands, PackageRegistry& reg, PersistenceManager& man) {
    commands["exit"] = std::make_unique<ExitCommand>();
    commands["login"] = std::make_unique<LoginCommand>();
    commands["save"] = std::make_unique<SaveCommand>(man, reg);
    commands["load"] = std::make_unique<LoadCommand>(man, reg);
    commands["pwd"] = std::make_unique<PWDCommand>();
    commands["chdir"] = std::make_unique<ChdirCommand>();
    commands["list"] = std::make_unique<ListCommand>();
    commands["webget"] = std::make_unique<WebgetCommand>();
    commands["mkdir"] = std::make_unique<MkdirCommand>();
    commands["touch"] = std::make_unique<TouchCommand>();
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

    shell.run();
}
