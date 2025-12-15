#pragma once
#include <memory>
#include "./kernel/Kernel.hpp"
#include "./services/Services.hpp"
#include "./io/IO.hpp"

struct Context {
    Kernel& kernel;
    Services& services;
    IO& io;
    std::unique_ptr<Shell> shell;
};