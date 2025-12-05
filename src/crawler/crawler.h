#pragma once

#include "iostream"
#include "vector"
#include "string"
#include "chrono"
#include "thread"
#include "cstdio"
#include "unordered_map"

#include "cpr/cpr.h"

class Crawler {
    public:
        int threads_T{1}; // Number of threads to use / default: 1
        std::chrono::seconds timeout_T; // Timeout for each request
        std::vector<std::string> collection_T; // Collection of domains
        std::unordered_map<std::string, std::string> headers_T; // Headers for each request
        std::vector<std::string> results_T; // Vector to store the results
        /**
         * @brief Returns a static instance of the Crawler class.
         * @return A pointer to a static instance of the Crawler class.
         * @details This function returns a pointer to a static instance of the Crawler class.
         * It is used to retrieve a static instance of the Crawler class.
         */
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

            printf("Looking at %s\n", url.c_str());

            // print out user agent
            cpr::Header header = cpr::Header(this->headers_T.begin(), this->headers_T.end());
            cpr::Response r = cpr::Get(cpr::Url{url}, cpr::Timeout{this->timeout_T}, header);
            if (r.status_code == 200) {
                // checking if there is ref: refs/heads/ in the content
                if (r.text.find("ref: refs/heads/") != std::string::npos) {
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
            // validating threads, to avoid threads in negative or zero threads.
            const size_t threads = (this->threads_T > 0) ? this->threads_T : 1;
            
            // checking if threads are not more than items in the collection
            if (threads > this->collection_T.size()) {
                // dynamically setting threads to the size of the collection
                //      to prevent overthreading
                this->threads_T = this->collection_T.size();
            }
            
            // splitting collection in chunks for each thread
            const size_t total = this->collection_T.size();
            const size_t chunk_size = total / this->threads_T;
            const size_t remainder = total % this->threads_T;

            std::vector<std::thread> pool;
            size_t start = 0;

            for (int i = 0; i < this->threads_T; i++) {
                std::cout << "Thread " << i << " started" << std::endl;
                size_t end = start + chunk_size;
                if (i == this->threads_T - 1) {
                    end += remainder;
                }
                pool.emplace_back([this, start, end] {
                    // scanning each chunk
                    for (size_t j = start; j < end; j++) {
                        scan(this->collection_T[j]);
                    }
                });
                // updating start for next chunk
                start = end;
                if (start >= total) break; // break if start is greater than or equal to total
            }

            // joining threads
            for (auto &thread : pool) {
                thread.join();
            }

            // printing out results

            printf("\n====== RESULTS ======\n");
            printf("[+] Total scanned %zu domains\n", total);

            if (this->results_T.empty()) {
                printf("[+] No results found\n");
                return 0;
            }
            printf("[+] Found %zu results\n", this->results_T.size());
            printf("\n");
            for (auto& result : this->results_T) {
                std::cout << result << std::endl;
            }
            return 0;
        }
};