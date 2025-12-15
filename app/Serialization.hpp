#pragma once
#include <cstdint>
#include <string>

class Serializer {
public:
    virtual void writeU32(uint32_t) = 0;
    virtual void writeString(const std::string&) = 0;
    virtual void writeBytes(const void*, size_t) = 0;
};

class Deserializer {
public:
    virtual uint32_t readU32() = 0;
    virtual std::string readString() = 0;
    virtual void readBytes(void*, size_t) = 0;
};
