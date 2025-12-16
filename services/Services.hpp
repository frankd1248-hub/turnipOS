#pragma once
#include <memory>
#include "AuthService.hpp"
#include "LogService.hpp"
#include "FileService.hpp"
#include "WebService.hpp"

struct Services {
    std::unique_ptr<AuthService> auth;
    std::unique_ptr<LogService>  log;
    std::unique_ptr<FileService> files;
    std::unique_ptr<WebService>  web;
};
