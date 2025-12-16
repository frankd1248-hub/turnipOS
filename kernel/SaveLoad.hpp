#pragma once
#include <string>
#include <sstream>
#include "../kernel/Kernel.hpp"
#include "../app/AppRegistry.hpp"
#include "../app/App.hpp"
#include "../Context.hpp"

inline std::string saveSystem(Context& ctx) {
    std::ostringstream out;
    out << ctx.kernel.state().serialize();
    return out.str();
}

inline void loadSystem(Context& ctx, const std::string& data) {
    ctx.kernel.state() = SystemState::deserialize(data);
}