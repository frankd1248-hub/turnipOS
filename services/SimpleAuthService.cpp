#include "SimpleAuthService.hpp"

bool SimpleAuthService::login(const std::string& user, const std::string& pass) {
    if (user == "root") {
        m_admin = true;
    }
    return true;
}

bool SimpleAuthService::isAdmin() const {
    return m_admin;
}