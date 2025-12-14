#include "AuthService.hpp"

class SimpleAuthService : public AuthService {

public:
    bool login(const std::string& user, const std::string& pass) override;
    bool isAdmin() const override;

private:
    bool m_admin = false;
};