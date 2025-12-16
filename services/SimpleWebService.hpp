#pragma once
#include <stdexcept>
#include "WebService.hpp"
#include "../web/Webget.hpp"

class SimpleWebService : public WebService {
public:

    void downloadFileToPath(const std::string& url, const std::filesystem::path& path) override {
        try {
            downloadFile(url, path);
        } catch (std::runtime_error exc) {
            throw exc;
        }
    }

private:
};