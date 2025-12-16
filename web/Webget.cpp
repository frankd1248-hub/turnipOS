#include "Webget.hpp"

size_t Webgetter::curlWriteCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    auto* file = static_cast<DownloadedFile*>(userdata);

    const size_t totalBytes = size * nmemb;
    file->append(ptr, totalBytes);

    return totalBytes;
}

DownloadedFile Webgetter::downloadFile(const std::string& url) {
    CURL* curl = curl_easy_init();
    if (!curl)
        throw std::runtime_error("Failed to init curl");

    DownloadedFile file(url, "downloaded_file");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        throw std::runtime_error(curl_easy_strerror(res));
    }

    curl_easy_cleanup(curl);
    return file;
}