#pragma once
#include <unordered_map>
#include "AuthService.hpp"
#include "LocalFileService.hpp"

class SimpleAuthService : public AuthService {

public:
    SimpleAuthService() : passwords() { passwords.emplace("root", rootpasswd); }

    bool login(const std::string& user, const std::string& pass) override;
    bool isAdmin() const override;

    const std::unordered_map<std::string, std::string>& getUserPasswordPairs() const override { return passwords; }

private:
    bool m_admin = false;
    LocalFileService fileio = LocalFileService();

    const std::string rootpasswd = "TURNIPROOT";
    std::unordered_map<std::string, std::string> passwords;
};
