#pragma once
#include "WebService.hpp"
#include "../web/Webget.hpp"

class SimpleWebService : public WebService {
public:

    void downloadFileToPath(const std::string& url, const std::filesystem::path& path) override {
        DownloadedFile file = get.downloadFile(url);
        file.writeToDisk(path);
    }

private:
    Webgetter get{};
};