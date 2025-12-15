#pragma once
#include <filesystem>
#include "TPK.hpp"

class TpkPacker {
public:
    void addChunk(const std::string& type,
                  const std::vector<uint8_t>& data,
                  uint32_t flags = 0);

    void pack(const std::string& outputPath);

private:
    std::vector<Chunk> m_chunks;
};

class TpkUnpacker {
public:
    explicit TpkUnpacker(const std::string& path);

    bool hasChunk(const std::string& type) const;
    std::vector<uint8_t> readChunk(const std::string& type);

private:
    TpkHeader m_header{};
    std::vector<TpkChunkEntry> m_table;
    std::ifstream m_in;
};
