#pragma once
#include <curl/curl.h>
#include "DownloadedFile.hpp"

size_t curlWriteCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    auto* file = static_cast<DownloadedFile*>(userdata);

    const size_t totalBytes = size * nmemb;
    file->append(ptr, totalBytes);

    return totalBytes;
}

// The download function
inline DownloadedFile downloadFile(const std::string& url, const std::filesystem::path& outputPath) {
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