#include "iostream"
#include "string"
#include "cpr/cpr.h"
#include "vector"
#include "chrono"

#include "collector/collector.h"
#include "crawler/crawler.h"

using namespace std::chrono_literals;

/**
 * @brief Main function.
 * This function collects a list of URLs from a file, sets up a crawler with 10 threads, and runs it.
 * The crawler will scan each URL and check if it is a valid git repository.
 * If it is, it will print out "Found git repo" and add the URL to the results vector.
 */
int main(){
    // collecting urls from file
    Collector collector;
    std::string collectorFile = "src/urls.txt";
    std::vector<std::string> domains = collector.LoadFromFile(collectorFile);

    // running crawler with 10 threads
    Crawler::setup()
        ->timeout(30s)
        ->threads(20)
        ->headers({
            {"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/237.84.2.178 Safari/537.36"}
        })
        ->collection(domains)
        ->Run();
    return 0;
}