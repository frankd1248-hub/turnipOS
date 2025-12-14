#include "LoginCommand.hpp"

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