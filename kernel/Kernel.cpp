#include "Kernel.hpp"

Kernel::Kernel(SystemState state) : m_state(std::move(state)) {}

void Kernel::shutdown() {
    m_state.running = false;
}

SystemState& Kernel::state() {
    return m_state;
}

void Kernel::setState(SystemState state) {
    m_state = std::move(state);
}