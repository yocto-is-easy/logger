#pragma once

#include <ctime>
#include <string>
#include <chrono>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;
namespace chrono = std::chrono;

class file_logger
{
private:
    std::string m_logFilePath;

    std::string get_timestamp() {
        auto now = chrono::system_clock::now();

        auto ms = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()) % 1000;
        auto mcs = chrono::duration_cast<chrono::microseconds>(now.time_since_epoch()) % 1000;
        auto ns = chrono::duration_cast<chrono::nanoseconds>(now.time_since_epoch()) % 1000;

        std::time_t t = std::time(nullptr);
        std::tm tm = *std::gmtime(&t);

        std::stringstream ss;
        ss << std::put_time(&tm, "%D %T");
        ss << "." << std::setfill('0') << std::setw(3) << ms.count();
        ss << "." << std::setfill('0') << std::setw(3) << mcs.count();
        ss << "." << std::setfill('0') << std::setw(3) << ns.count();

        return ss.str();
    }

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

        logFile.open(m_logFilePath, std::fstream::app);
        if(logFile.is_open()) {
            std::string data = "[" + get_timestamp() + "] " + msg + "\n";
            logFile << data;
            logFile.close();
            return;
        }
    }
};
