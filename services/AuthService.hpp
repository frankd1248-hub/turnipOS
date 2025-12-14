#pragma once
#include <string>

class AuthService {
public:
    virtual ~AuthService() = default;
    virtual bool login(const std::string& user, const std::string& pass) = 0;
    virtual bool isAdmin() const = 0;
};