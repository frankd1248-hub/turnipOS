#include "./kernel/Kernel.hpp"
#include "./services/Services.hpp"
#include "./services/SimpleAuthService.hpp"
#include "./services/FileLogService.hpp"
#include "./shell/Shell.hpp"
#include "./commands/DefaultCommands.hpp"
#include "./io/ConsoleIO.hpp"

int main(int argc, char** argv) {
    SystemState state;
    state.deviceName = "turnipOS";
    Kernel kernel(state);

    Services services;
    services.auth = std::make_unique<SimpleAuthService>();
    services.log = std::make_unique<FileLogService>();

    ConsoleIO io;
    Context ctx{kernel, services, io};

    CommandMap commands;
    commands["exit"] = std::make_unique<ExitCommand>();
    commands["login"] = std::make_unique<LoginCommand>();

    Shell shell(std::move(commands), io, ctx);

    shell.run();
}
