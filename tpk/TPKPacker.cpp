#include <cstring>
#include "TPKPacker.hpp"

void TpkPacker::addChunk(const std::string& type,
                         const std::vector<uint8_t>& data,
                         uint32_t flags) {
    if (type.size() != 4)
        throw std::runtime_error("Chunk type must be 4 chars");

    m_chunks.push_back({type, data, flags});
}

void TpkPacker::pack(const std::string& outputPath) {
    std::ofstream out(outputPath, std::ios::binary);
    if (!out) throw std::runtime_error("Cannot open output");

    TpkHeader header{};
    memcpy(header.magic, "TPK\0", 4);
    header.major = 1;
    header.minor = 0;
    header.chunkCount = m_chunks.size();
    header.chunkTableOfs = sizeof(TpkHeader);

    writeStruct(out, header);

    // Reserve chunk table
    std::vector<TpkChunkEntry> table(m_chunks.size());
    out.seekp(sizeof(TpkChunkEntry) * m_chunks.size(), std::ios::cur);

    // Write chunks
    for (size_t i = 0; i < m_chunks.size(); ++i) {
        auto& c = m_chunks[i];

        TpkChunkHeader ch{};
        memcpy(ch.type, c.type.data(), 4);
        ch.size = c.data.size();
        ch.flags = c.flags;

        uint32_t payloadOfs = out.tellp();
        writeStruct(out, ch);
        out.write(reinterpret_cast<char*>(c.data.data()), c.data.size());

        memcpy(table[i].type, ch.type, 4);
        table[i].offset = payloadOfs;
        table[i].size = ch.size;
    }

    // Write chunk table
    out.seekp(header.chunkTableOfs);
    for (auto& e : table)
        writeStruct(out, e);
}

TpkUnpacker::TpkUnpacker(const std::string& path)
    : m_in(path, std::ios::binary) {
    if (!m_in) throw std::runtime_error("Cannot open TPK");

    readStruct(m_in, m_header);

    if (memcmp(m_header.magic, "TPK\0", 4) != 0)
        throw std::runtime_error("Invalid TPK magic");

    if (m_header.major != 1)
        throw std::runtime_error("Unsupported TPK version");

    m_in.seekg(m_header.chunkTableOfs);
    m_table.resize(m_header.chunkCount);

    for (auto& e : m_table)
        readStruct(m_in, e);
}

std::vector<uint8_t> TpkUnpacker::readChunk(const std::string& type) {
    for (const auto& e : m_table) {
        if (type == std::string(e.type, 4)) {
            m_in.seekg(e.offset);
            TpkChunkHeader ch;
            readStruct(m_in, ch);

            std::vector<uint8_t> data(ch.size);
            m_in.read(reinterpret_cast<char*>(data.data()), ch.size);
            return data;
        }
    }
    throw std::runtime_error("Chunk not found");
}

bool TpkUnpacker::hasChunk(const std::string& type) const {
    if (type.size() != 4)
        return false;

    for (const auto& entry : m_table) {
        if (std::memcmp(entry.type, type.data(), 4) == 0)
            return true;
    }
    return false;
}
