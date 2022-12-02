#pragma once

#include <string>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

class file_logger
{
private:
    std::string m_logFilePath;
public:
    file_logger(std::string logFilePath = "/dev/kmsg")
        : m_logFilePath(logFilePath) {
        if(!fs::exists(fs::path(m_logFilePath))) {
            fs::create_directories(fs::path(m_logFilePath).parent_path());
            std::ofstream log_file(m_logFilePath);
        }
    }

    void log(std::string msg) {
        std::ofstream logFile;

        logFile.open(m_logFilePath);
        if(logFile.is_open()) {
            logFile << (msg + "\n");
            logFile.close();
            return;
        }
    }
};
