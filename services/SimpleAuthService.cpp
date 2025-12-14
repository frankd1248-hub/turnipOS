#include "SimpleAuthService.hpp"
#include "../Utils.hpp"

bool SimpleAuthService::login(const std::string& user, const std::string& pass) {
    if (user == "root") {
        if (pass == rootpasswd) {
            m_admin = true;
            return true;
        }
    } else {
        if (fileio.read(concat("./passwds/passwd_", user)) == pass) {
            m_admin = false;
            return true;
        }
    }
    return false;
}

bool SimpleAuthService::isAdmin() const {
    return m_admin;
}
