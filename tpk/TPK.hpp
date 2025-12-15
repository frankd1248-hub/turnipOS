#pragma once
#include <cstdint>
#include <fstream>
#include <vector>

struct TpkChunkHeader {
    char     type[4];   // e.g. "MANF", "CODE"
    uint32_t size;      // payload size in bytes
    uint32_t flags;     // bitfield (compressed, encrypted, etc.)
    uint32_t reserved;  // future use (must be 0)
};

struct TpkHeader {
    char     magic[4];     // "TPK\0"
    uint16_t major;        // format major
    uint16_t minor;        // format minor
    uint32_t flags;        // global flags
    uint32_t chunkCount;   // number of chunks
    uint32_t chunkTableOfs;// offset to chunk table
};

struct TpkChunkEntry {
    char     type[4];
    uint32_t offset;
    uint32_t size;
};

struct HashPayload {
    uint8_t algorithm;  // e.g. 1 = SHA256
    uint8_t hash[32];
};

struct Chunk {
    std::string type;           // 4 chars
    std::vector<uint8_t> data;
    uint32_t flags = 0;
};

template<typename T>
void writeStruct(std::ofstream& out, const T& s) {
    out.write(reinterpret_cast<const char*>(&s), sizeof(T));
}

template<typename T>
void readStruct(std::ifstream& in, T& s) {
    in.read(reinterpret_cast<char*>(&s), sizeof(T));
}