#include "NetworkManager.h"
#include <iostream>

size_t NetworkManager::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    auto* output = static_cast<std::string*>(userp);
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

NetworkManager::NetworkManager() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize CURL" << std::endl;
    }
}

NetworkManager::~NetworkManager() {
    if (curl) {
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

std::string NetworkManager::fetchPage(const std::string& url) {
    if (!curl) {
        return "Error: CURL not initialized";
    }

    std::string response;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        return "Error: " + std::string(curl_easy_strerror(res));
    }

    return response;
}
