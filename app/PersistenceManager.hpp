#pragma once
#include <string>
#include "Package.hpp"

class PersistenceManager {
public:
    void savePackage(const Package& pkg);
    bool loadPackage(Package& pkg);

private:
    std::filesystem::path statePathFor(const Package& pkg) const;
};
