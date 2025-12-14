#pragma once
#include "SystemState.hpp"

class Kernel {

public:
    explicit Kernel(SystemState state);

    void shutdown();
    SystemState& state();

private:
    SystemState m_state;
};