#pragma once
#include <string>
#include <vector>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <stdexcept>

class DownloadedFile {
public:
    using Byte = std::byte;

    DownloadedFile(std::string sourceUrl, std::string filename, std::string mimeType = {})
        : m_url(std::move(sourceUrl)),
          m_filename(std::move(filename)),
          m_mimeType(std::move(mimeType)) {}

    void append(const void* data, std::size_t size) {
        const Byte* bytes = static_cast<const Byte*>(data);
        m_data.insert(m_data.end(), bytes, bytes + size);
    }

    const std::vector<Byte>& data() const noexcept { return m_data; }
    std::size_t size() const noexcept { return m_data.size(); }

    const std::string& url() const noexcept { return m_url; }
    const std::string& filename() const noexcept { return m_filename; }
    const std::string& mimeType() const noexcept { return m_mimeType; }

    void writeToDisk(const std::filesystem::path& path) const {

        if (m_data.empty())
            throw std::runtime_error("DownloadedFile is empty");

        // Ensure parent directory exists (if any)
        if (path.has_parent_path()) {
            std::error_code ec;
            std::filesystem::create_directories(path.parent_path(), ec);
            if (ec) {
                throw std::runtime_error(
                    "Failed to create directories: " + ec.message());
            }
        }

        std::ofstream out(path, std::ios::binary | std::ios::trunc);
        if (!out)
            throw std::runtime_error("Failed to open file for writing");

        out.write(reinterpret_cast<const char*>(m_data.data()),
              static_cast<std::streamsize>(m_data.size()));

        if (!out)
            throw std::runtime_error("Failed while writing file to disk");
    }

private:
    std::string m_url;
    std::string m_filename;
    std::string m_mimeType;
    std::vector<Byte> m_data;
};
