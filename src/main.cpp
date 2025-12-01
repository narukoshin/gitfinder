#include "iostream"
#include "string"
#include "chrono"
#include "unordered_map"
#include "collector/collector.h"
#include "crawler/crawler.h"
#include "config/config.h"

using namespace std::chrono_literals;

/**
 * @brief The main function of the program.
 * This function reads the configuration from config.yml,
 * loads the collection of urls from the specified file,
 * and runs the crawler with the specified number of threads and timeout.
 * @return 0 if the program runs successfully, 1 otherwise.
 */
int main(){
    int threads;
    std::string timeout, collection_file;
    std::unordered_map<std::string, std::string> headers;

    try {
        // reading config file
        Config::load("config.yml")
            ->parameter("threads", &threads)
            ->parameter("timeout", &timeout)
            ->parameter("collection_file", &collection_file)
            ->parameter("headers", &headers);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    // converting timeout string to seconds
    std::chrono::seconds seconds = std::chrono::seconds(std::stoi(timeout));

    // collecting urls from file
    auto collection = Collector::FromFile(collection_file);

    // running crawler with specified parameters
    return Crawler::setup()
        ->timeout(seconds)
        ->threads(threads)
        ->headers(headers)
        ->collection(collection)
        ->Run();
}