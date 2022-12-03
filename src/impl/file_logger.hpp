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
    lrrp::thread_pool<std::string> m_asyncMessageWriter;

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

    std::string format_msg(const std::string& str) {
        return "[" + get_timestamp() + "] " + str + "\n";
    }

public:
    file_logger(std::string logFilePath = "/dev/kmsg")
        : m_logFilePath(logFilePath)
        , m_asyncMessageWriter(1) {
        m_asyncMessageWriter.set_worker([this](std::string&& data) { this->log_writer(std::move(data)); });
        if(!fs::exists(fs::path(m_logFilePath))) {
            fs::create_directories(fs::path(m_logFilePath).parent_path());
            std::ofstream log_file(m_logFilePath);
        }
    }

    void log_writer(std::string&& data) {
        std::ofstream logFile;

        logFile.open(m_logFilePath, std::fstream::app);
        if(logFile.is_open()) {
            logFile << data;
            logFile.close();
        }
    }

    void log(const std::string& msg) {
        log_writer(format_msg(msg));
    }

    void log_async(const std::string& msg) {
        std::string data = format_msg(msg);
        m_asyncMessageWriter.enqueue(std::move(data));
    }
};
