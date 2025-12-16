#pragma once
#include <string>
#include <filesystem>

class WebService {
public:
    virtual ~WebService() = default;
    virtual void downloadFileToPath(const std::string& url, const std::filesystem::path& path) = 0;
};