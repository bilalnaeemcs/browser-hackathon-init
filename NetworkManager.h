#pragma once
#include <string>
#include <curl/curl.h>

class NetworkManager {
public:
    NetworkManager();
    ~NetworkManager();
    std::string fetchPage(const std::string& url);

private:
    CURL* curl;
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
};
