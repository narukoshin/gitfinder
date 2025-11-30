#pragma once

#include "iostream"
#include "string"
#include "filesystem"
#include "vector"

class Collector {
private:
    std::vector<std::string> urls; // vector to store the urls

public:
    /**
     * @brief Loads URLs from a file and stores them in a vector.
     * @param file The path to the file to load from.
     * @return A vector of strings containing the URLs from the file.
     * @details This function loads URLs from a file and stores them in a vector. If the file does not exist, it prints "File not found" and returns an empty vector.
     */
    std::vector<std::string> LoadFromFile(std::string file){
        // checking if the file exists
        if (!std::filesystem::exists(file)){
            std::cout << "File not found" << std::endl;
            return std::vector<std::string>{};
        }
        // reading the file
        std::ifstream fileStream(file);
        std::string line;
        while (std::getline(fileStream, line)) {
            urls.push_back(line);
        }
        return urls;
    }
};