#include "iostream"
#include "cpr/cpr.h"
#include "string"
#include <yaml-cpp/yaml.h>

std::string ConfigFile = "config.yml";

/**
 * @brief Reads the configuration file and prints out the value of the "test" variable.
 * @return 0 if the file is found and read successfully, 1 if the file is not found.
 * @details This function reads the configuration file specified by the ConfigFile variable and prints out the value of the "test" variable. If the file is not found, it prints an error message and returns 1.
 */
int read_config(){
    if (!std::filesystem::exists(ConfigFile)){
        std::cout << "Config file not found" << std::endl;
        return 1;
    }

    YAML::Node config = YAML::LoadFile(ConfigFile);
    std::cout << config["test"] << std::endl;
    return 0;
}

/**
 * @brief A simple program that sends a GET request to a given URL and 
 *           prints the response if the status code is not 403.
 * @return 0
 */
int main(){
    cpr::Header header = cpr::Header{{"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/237.84.2.178 Safari/537.36"}};
    cpr::Response r = cpr::Get(cpr::Url{"https://mail.narukoshin.me"}, header);
    if (r.status_code != 403){
        std::cout << r.text << std::endl;
    } else {
        std::cout << "403 Forbidden" << std::endl;
    }
    return 0;
}