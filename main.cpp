#include "./kernel/Kernel.hpp"
#include "./services/Services.hpp"
#include "./services/SimpleAuthService.hpp"
#include "./services/FileLogService.hpp"
#include "./services/LocalFileService.hpp"
#include "./shell/Shell.hpp"
#include "./shell/CommandRegistry.hpp"
#include "./commands/Command.hpp"
#include "./io/ConsoleIO.hpp"

int main(int argc, char** argv) {
    CommandMap commands;
    commands["exit"] = std::make_unique<ExitCommand>();
    commands["login"] = std::make_unique<LoginCommand>();
    commands["save"] = std::make_unique<SaveCommand>();
    commands["load"] = std::make_unique<LoadCommand>();
    commands["pwd"] = std::make_unique<PWDCommand>();

    SystemState state;
    state.deviceName = "turnipOS";
    state.pwd = std::string(std::getenv("HOME")).append("/turnipOS");
    std::filesystem::current_path(state.pwd);
    Kernel kernel(state);

    Services services;
    services.auth = std::make_unique<SimpleAuthService>();
    services.log = std::make_unique<FileLogService>();
    services.files = std::make_unique<LocalFileService>();

    ConsoleIO io;
    Context ctx{kernel, services, io};
    Shell shell(std::move(commands), io, ctx);

    shell.run();
}
