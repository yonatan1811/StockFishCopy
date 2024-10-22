#ifndef LICHESS_API_H
#define LICHESS_API_H

#include <string>
#include <iostream>
#include "C:/Users/noam/Downloads/vcpkg/packages/curl_x64-windows/include/curl/curl.h"
#include <functional>
#include <thread>

class LichessAPI {
public:
    LichessAPI(const std::string& token);
    void startListening(std::function<void(const std::string&, const std::string&)> callback);
    void sendMove(const std::string& game_id, const std::string& move);

private:
    std::string token;
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
};

#endif // LICHESS_API_H
