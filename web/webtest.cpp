#include <curl/curl.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

class DownloadedFile {
public:
    using Byte = std::byte;

    DownloadedFile(std::string sourceUrl, std::string filename)
        : m_url(std::move(sourceUrl)), m_filename(std::move(filename)) {}

    void append(const void* data, std::size_t size) {
        const Byte* bytes = static_cast<const Byte*>(data);
        m_data.insert(m_data.end(), bytes, bytes + size);
    }

    const std::vector<Byte>& data() const noexcept { return m_data; }

    void setMimeType(std::string type) { m_mimeType = std::move(type); }
    const std::string& mimeType() const noexcept { return m_mimeType; }

    const std::string& filename() const noexcept { return m_filename; }

    void writeToDisk(const std::filesystem::path& path) const {
        if (m_mimeType.starts_with("text/html")) {
            throw std::runtime_error("Server returned HTML instead of file content");
        }

        if (m_data.empty()) {
            throw std::runtime_error("DownloadedFile is empty");
        }

        if (path.has_parent_path()) {
            std::error_code ec;
            std::filesystem::create_directories(path.parent_path(), ec);
            if (ec)
                throw std::runtime_error("Failed to create directories: " + ec.message());
        }

        std::ofstream out(path, std::ios::binary | std::ios::trunc);
        if (!out) throw std::runtime_error("Failed to open file for writing");

        out.write(reinterpret_cast<const char*>(m_data.data()), static_cast<std::streamsize>(m_data.size()));
        if (!out) throw std::runtime_error("Failed while writing file to disk");
    }

private:
    std::string m_url;
    std::string m_filename;
    std::string m_mimeType;
    std::vector<Byte> m_data;
};

// libcurl write callback
static size_t curlWriteCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    auto* file = static_cast<DownloadedFile*>(userdata);
    size_t total = size * nmemb;
    file->append(ptr, total);
    return total;
}

// The download function
DownloadedFile downloadFile(const std::string& url, const std::filesystem::path& outputPath) {
    CURL* curl = curl_easy_init();
    if (!curl) throw std::runtime_error("Failed to initialize libcurl");

    DownloadedFile file(url, outputPath.filename().string());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);          // follow redirects
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0"); // standard User-Agent
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 0L);             // don’t fail on HTTP errors automatically

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        throw std::runtime_error(std::string("curl_easy_perform failed: ") + curl_easy_strerror(res));
    }

    // Check HTTP response code
    long httpStatus = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpStatus);
    if (httpStatus < 200 || httpStatus >= 300) {
        curl_easy_cleanup(curl);
        throw std::runtime_error("HTTP error " + std::to_string(httpStatus));
    }

    // Get MIME type
    char* contentType = nullptr;
    curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &contentType);
    if (contentType) file.setMimeType(contentType);

    curl_easy_cleanup(curl);

    // Write to disk
    file.writeToDisk(outputPath);

    return file;
}

// Example usage
// int main() {
//     try {
//         std::filesystem::path outPath = "./test6.bin";
//         DownloadedFile file = downloadFile("https://ash-speed.hetzner.com/100MB.bin", outPath);
//         std::cout << "Downloaded " << file.filename() << " (" << file.data().size() << " bytes)" << std::endl;
//     } catch (const std::exception& e) {
//         std::cerr << "Download failed: " << e.what() << std::endl;
//         return 1;
//     }
//     return 0;
// }
