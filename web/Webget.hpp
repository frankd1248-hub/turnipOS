#pragma once
#include <curl/curl.h>
#include "DownloadedFile.hpp"

class Webgetter {
public:
    DownloadedFile downloadFile(const std::string& url);

private:
    static size_t curlWriteCallback(char* ptr, size_t size, size_t nmemb, void* userdata);
};