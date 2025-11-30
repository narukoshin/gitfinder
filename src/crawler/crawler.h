#pragma once

#include "iostream"
#include "vector"
#include "string"
#include "chrono"
#include <thread>
#include "unordered_map"

#include "cpr/cpr.h"

class Crawler {
    public:
        int threads_T{1}; // Number of threads to use / default: 1
        std::chrono::seconds timeout_T; // Timeout for each request
        std::vector<std::string> collection_T; // Collection of domains
        std::unordered_map<std::string, std::string> headers_T; // Headers for each request
        std::vector<std::string> results_T; // Vector to store the results
        static Crawler* setup() {
            static Crawler c;
            return &c;
        }
        /**
         * @brief Sets the number of threads to use for crawling.
         * @param t The number of threads to use.
         * @return This Crawler object.
         * @details This function sets the number of threads to use for crawling and returns this Crawler object.
         */
        Crawler* threads(int t) {
            this->threads_T = t;
            return this;
        }
        /**
         * @brief Sets the timeout for each request.
         * @param timeout The timeout for each request in seconds.
         * @return This Crawler object.
         * @details This function sets the timeout for each request and returns this Crawler object.
         */
        Crawler* timeout(std::chrono::seconds timeout) {
            this->timeout_T = timeout;
            return this;
        }
        /**
         * @brief Sets the collection of domains to crawl.
         * @param c A vector of strings containing the domains to crawl.
         * @return This Crawler object.
         * @details This function sets the collection of domains to crawl and returns this Crawler object.
         */
        Crawler* collection(std::vector<std::string> c) {
            this->collection_T = c;
            return this;
        }
        /**
         * @brief Sets the headers for each request.
         * @param h An unordered map of strings containing the headers to set.
         * @return This Crawler object.
         * @details This function sets the headers for each request and returns this Crawler object.
         */
        Crawler* headers(std::unordered_map<std::string, std::string> h) {
            this->headers_T = h;
            return this;
        }
    private:
        /**
         * Scan a given URL and check if it is a valid git repository.
         * The function adds .git/HEAD to the end of the URL and makes a GET request.
         * If the response status code is 200, it checks if the content contains "ref: refs/heads/".
         * If it does, it prints out "Found git repo" and adds the URL to the results vector.
         *
         * @param url The URL to scan.
         */
        void scan(std::string url) {
            // adding .git/HEAD to the url end of first url
            url += "/.git/HEAD";

            std::cout << "Scanning " << url << std::endl;

            // print out user agent
            cpr::Header header = cpr::Header(this->headers_T.begin(), this->headers_T.end());
            cpr::Response r = cpr::Get(cpr::Url{url}, cpr::Timeout{this->timeout_T}, header);
            if (r.status_code == 200) {
                // checking if there is ref: refs/heads/ in the content
                if (r.text.find("ref: refs/heads/") != std::string::npos) {
                    std::cout << "Found git repo" << std::endl;
                    this->results_T.push_back(url);
                }
            }
        }
    public:
        /**
         * Runs the crawler.
         *
         * This function checks if there is any data in the collection, splits the collection in chunks for each thread,
         * creates a thread pool to scan each chunk, joins all threads, prints out the results and returns 0 if successful.
         * If the collection is empty, it prints out "Collection is empty" and returns 1.
         *
         * @return 0 if successful, 1 if the collection is empty.
         */
        int Run() {
            // checking if there is any data in the collection
            if (this->collection_T.empty()) {
                std::cout << "Collection is empty" << std::endl;
                return 1;
            }
            // splitting collection in chunks for each thread
            size_t total = this->collection_T.size();
            size_t chunk_sze = total / this->threads_T;
            size_t reminder = total % this->threads_T;

            std::vector<std::thread> pool;
            size_t start = 0;

            for (int i = 0; i < this->threads_T; i++) {
                size_t end = start + chunk_sze;
                if (i == this->threads_T - 1) {
                    end += reminder;
                }
                pool.push_back(std::thread([=] {
                    for (size_t j = start; j < end; j++) {
                        this->scan(this->collection_T[j]);
                    }
                }));
                start = end;
            }
            for (auto& thread : pool) {
                thread.join();
            }

            std::cout << "Found " << this->results_T.size() << " git repos" << std::endl;

            for (auto& result : this->results_T) {
                std::cout << result << std::endl;
            }
            return 0;
        }
};