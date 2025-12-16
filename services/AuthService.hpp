#pragma once
#include <string>
#include <unordered_map>

class AuthService {
public:
    virtual ~AuthService() = default;
    virtual bool login(const std::string& user, const std::string& pass) = 0;
    virtual bool isAdmin() const = 0;
    virtual const std::unordered_map<std::string, std::string>& getUserPasswordPairs() const = 0;
};