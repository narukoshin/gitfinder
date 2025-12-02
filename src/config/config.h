#pragma once

#include "string"
#include "yaml-cpp/yaml.h"

using namespace std::chrono_literals;

class Config {
    private:
        YAML::Node config; // variable to store the contents of the configuration file
        /**
         * @brief Reads the configuration file specified by the configName parameter and stores the contents in the config member variable.
         * @param configName The name of the configuration file to read.
         * @details This function reads the configuration file specified by the configName parameter and stores the contents in the config member variable.
         */
        void readConfig(std::string configName) {
            // Checking if the file exists
            if (!std::filesystem::exists(configName)) {
                std::cout << "Error: Config file not found" << std::endl;
                exit(1);
            }
            this->config = YAML::LoadFile(configName);
        }
    public:
        /**
         * @brief Loads the configuration file specified by the fileName parameter and returns a pointer to a loaded Config object.
         * @param fileName The name of the configuration file to load.
         * @return A pointer to a loaded Config object.
         * @details This function reads the configuration file specified by the fileName parameter and returns a pointer to a loaded Config object.
         */
        static Config* load(std::string fileName) {
            static Config c;
            c.readConfig(fileName);
            return &c;
        }
        /**
         * @brief Retrieves the value associated with a given parameter from the configuration file and stores it in the given pointer.
         * @param parameter The parameter to retrieve the value for.
         * @param ptr A pointer to store the retrieved value.
         * @return This Config object.
         * @details This function retrieves the value associated with a given parameter from the configuration file, stores it in the given pointer, and returns this Config object.
         * If the parameter is not found in the configuration file, it will print out an error message and return this Config object without modifying the given pointer.
         */
        template<typename T>
        Config* parameter(std::string parameter, T* ptr){
            if (!this->config[parameter]) {
                std::cerr << "Error: Parameter " << parameter << " not found" << std::endl;
                exit(1);
            }
            *ptr = this->config[parameter].as<T>();
            return this;
        }
};