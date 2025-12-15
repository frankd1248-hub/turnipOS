#pragma once
#include <fstream>
#include "Serialization.hpp"

class BinaryFileSerializer : public Serializer {
public:
    explicit BinaryFileSerializer(std::ofstream& out) : m_out(out) {}

    void writeU32(uint32_t v) override {
        m_out.write(reinterpret_cast<char*>(&v), sizeof(v));
    }

    void writeString(const std::string& s) override {
        writeU32(s.size());
        m_out.write(s.data(), s.size());
    }

    void writeBytes(const void* d, size_t n) override {
        m_out.write(reinterpret_cast<const char*>(d), n);
    }

private:
    std::ofstream& m_out;
};

class BinaryFileDeserializer : public Deserializer {
public:
    explicit BinaryFileDeserializer(std::ifstream& in)
        : m_in(in) {}

    uint32_t readU32() override {
        uint32_t v;
        readExact(&v, sizeof(v));
        return v;
    }

    std::string readString() override {
        uint32_t len = readU32();

        if (len > kMaxStringSize)
            throw std::runtime_error("String too large");

        std::string s(len, '\0');
        readExact(s.data(), len);
        return s;
    }

    void readBytes(void* dst, size_t n) override {
        readExact(dst, n);
    }

private:
    void readExact(void* dst, size_t n) {
        if (!m_in.read(reinterpret_cast<char*>(dst), n))
            throw std::runtime_error("Unexpected end of file");
    }

private:
    std::ifstream& m_in;
    static constexpr uint32_t kMaxStringSize = 1024 * 1024; // 1 MB guard
};