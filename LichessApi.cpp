#include "lichess_api.h"

LichessAPI::LichessAPI(const std::string& token) : token(token) {}

size_t LichessAPI::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    // Handle response from Lichess here if needed
    return size * nmemb;
}

void LichessAPI::startListening(std::function<void(const std::string&, const std::string&)> callback)
{
    std::thread([this, callback]() {
        std::string url = "https://lichess.org/api/stream/game/";
        CURL *curl;
        CURLcode res;

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();

        if (curl) {
            // Set the URL for the request
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            
            // Set up authentication headers
            struct curl_slist *headers = NULL;
            headers = curl_slist_append(headers, ("Authorization: Bearer " + token).c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            
            // Set callback function for receiving data
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, nullptr); // Use default write function
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, nullptr);    // Use default write data

            // Make a loop to keep receiving data
            while (true) {
                res = curl_easy_perform(curl);
                if (res != CURLE_OK) {
                    std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
                }

                // Sleep for a short time to avoid hammering the server
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            // Clean up
            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
        }

        curl_global_cleanup();
    }).detach();
}


void LichessAPI::sendMove(const std::string& game_id, const std::string& move) {
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        std::string url = "https://lichess.org/api/bot/game/" + game_id + "/move/" + move;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, "Authorization: Bearer " + token);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}
