#include "AuthService.hpp"
#include "LocalFileService.hpp"

class SimpleAuthService : public AuthService {

public:
    bool login(const std::string& user, const std::string& pass) override;
    bool isAdmin() const override;

private:
    bool m_admin = false;
    LocalFileService fileio = LocalFileService();

    const std::string rootpasswd = "TURNIPROOT";
};
