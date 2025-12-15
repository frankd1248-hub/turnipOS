#pragma once
#include "SystemState.hpp"

class Kernel {

public:
    explicit Kernel(SystemState state);

    void shutdown();
    SystemState& state();
    void setState(SystemState);

private:
    SystemState m_state;
};