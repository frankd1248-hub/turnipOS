#include "ExitCommand.hpp"
#include "../Utils.hpp"

void ExitCommand::execute(Context& ctx, const std::vector<std::string>& args) {
    ctx.kernel.shutdown();
}