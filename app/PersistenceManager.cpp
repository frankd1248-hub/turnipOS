#include "PersistenceManager.hpp"
#include "BinarySerialization.hpp"
#include "Package.hpp"
#include <sstream>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <cstring>

void PersistenceManager::savePackage(const Package& pkg) {
    auto path = statePathFor(pkg);
    auto tmp  = path.string() + ".tmp";

    std::ofstream out(tmp, std::ios::binary);
    if (!out) throw std::runtime_error("Save failed");

    // Header
    out.write("TPKS", 4);

    uint32_t formatVersion = 1;
    out.write(reinterpret_cast<char*>(&formatVersion), 4);

    uint32_t pkgVersion = pkg.stateVersion();
    out.write(reinterpret_cast<char*>(&pkgVersion), 4);

    BinaryFileSerializer ser(out);
    pkg.save(ser);

    out.close();
    std::filesystem::rename(tmp, path);
}

bool PersistenceManager::loadPackage(Package& pkg) {
    auto path = statePathFor(pkg);
    if (!std::filesystem::exists(path))
        return false;

    std::ifstream in(path, std::ios::binary);
    if (!in) return false;

    char magic[4];
    in.read(magic, 4);
    if (memcmp(magic, "TPKS", 4) != 0)
        return false;

    uint32_t formatVersion;
    in.read(reinterpret_cast<char*>(&formatVersion), 4);

    uint32_t savedPkgVersion;
    in.read(reinterpret_cast<char*>(&savedPkgVersion), 4);

    BinaryFileDeserializer deser(in);

    if (savedPkgVersion < pkg.stateVersion()) {
        pkg.load(deser);
        pkg.migrate(savedPkgVersion);
    } else {
        pkg.load(deser);
    }

    return true;
}

std::filesystem::path PersistenceManager::statePathFor(const Package& pkg) const {
    static const std::filesystem::path root =
        "/var/turnipOS/state";

    std::string safeName = pkg.name();

    for (char& c : safeName) {
        if (!std::isalnum(static_cast<unsigned char>(c)) && c != '_')
            c = '_';
    }

    return root / (safeName + ".pkgstate");
}