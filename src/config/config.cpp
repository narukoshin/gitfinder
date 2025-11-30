#include "string"
#include "iostream"
#include <yaml-cpp/yaml.h>

class Config {
    private:
        // variable to store the contents of the configuration file
        YAML::Node config;
        /**
         * @brief Reads the configuration file specified by the configName parameter and stores the contents in the config member variable.
         * @param configName The name of the configuration file to read.
         * @details This function reads the configuration file specified by the configName parameter and stores the contents in the config member variable.
         */
        void readConfig(std::string configName) {
            // Checking if the file exists
            if (!std::filesystem::exists(configName)) {
                std::cout << "Config file not found" << std::endl;
                return;
            }
            this->config = YAML::LoadFile(configName);
        }

    public:
        /**
         * @brief Constructor for Config class.
         * @param configName The name of the configuration file to read.
         * @details This constructor reads the configuration file specified by the configName parameter and stores the contents in the config member variable.
         */
        Config(std::string configName) {
            this->readConfig(configName);

        };
        /**
         * @brief Retrieves the value associated with a given key from the configuration file.
         * @param key The key to retrieve the value for.
         * @return The value associated with the given key.
         * @details This function retrieves the value associated with a given key from the configuration file and returns it as a std::string.
         */
        std::string getValue(std::string key) {
            return this->config[key].as<std::string>();
        }
};