#include "generator.h"
#include "libs.h"
#include <curl/curl.h>

const string WORD_URL = "https://raw.githubusercontent.com/dwyl/english-words/refs/heads/master/words.txt";
const string OUTPUT_FILE = "data/words.txt";

// Callback function to handle data received from the server
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userData) {
    size_t totalSize = size * nmemb;
    userData->append((char*)contents, totalSize);
    return totalSize;
}

void fetchWordsFromURL(const std::string& url = WORD_URL, const std::string& outputFile = OUTPUT_FILE) {
    CURL* curl;
    CURLcode res;

    std::string responseData; // To store the response from the server

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

        // Perform the request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
        }
        else {
            // Write the response data to a file
            std::ofstream outFile(outputFile);
            if (outFile.is_open()) {
                outFile << responseData;
                std::cout << "Words successfully stored in " << outputFile << std::endl;
            }
            else {
                std::cerr << "Error: Unable to open file " << outputFile << " for writing." << std::endl;
            }
        }

        // Clean up
        curl_easy_cleanup(curl);
    }
    else {
        std::cerr << "Error: Failed to initialize CURL." << std::endl;
    }
}

void getWords() {
    fetchWordsFromURL(WORD_URL, OUTPUT_FILE);
}