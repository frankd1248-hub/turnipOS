#pragma once
#include <unordered_map>
#include <memory>
#include "Command.hpp"

using CommandMap = std::unordered_map<std::string, std::unique_ptr<Command>>;