#include <iostream>
#include <fstream>
#include "SimpleAuthService.hpp"
#include "../Utils.hpp"

bool SimpleAuthService::login(const std::string& user, const std::string& pass) {
    if (user == "root") {
        if (pass == rootpasswd) {
            m_admin = true;
            return true;
        }
    } else {
        if (!passwords.contains(user)) {
            return false;
        } if (passwords.at(user) == pass) {
            m_admin = false;
            return true;
        }
    }
    return false;
}

bool SimpleAuthService::isAdmin() const {
    return m_admin;
}
