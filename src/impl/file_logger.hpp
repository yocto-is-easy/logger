#pragma once

#include <ctime>
#include <string>
#include <chrono>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;
namespace chrono = std::chrono;

enum class time_precision {
    NoFloat, // does not use float time
    Ms, //      milliseconds
    Mcs, //     microseconds
    Ns //       nanoseconds
};

const fs::path DEV_NULL = "/dev/null";
const uint64_t DEFAULT_MAX_FILE_SIZE = 100'000; // 100KB

class file_logger
{
private:
    lrrp::thread_pool<std::string> m_asyncMessageWriter;
    fs::path m_clearFilePath;
    fs::path m_currentLogFilePath; // current one
    uint64_t m_currentFileSize;
    fs::path m_prevLogFile;
    uint64_t m_maxFileSize;

public:
    file_logger(const std::string& logFilePath = "/dev/kmsg")
        : m_asyncMessageWriter(1)
        , m_clearFilePath(logFilePath)
        , m_currentLogFilePath(format_path(m_clearFilePath))
        , m_currentFileSize(0)
        , m_prevLogFile(DEV_NULL)
        , m_maxFileSize(DEFAULT_MAX_FILE_SIZE) {
        m_asyncMessageWriter.set_worker([this](std::string&& data) { this->log_writer(std::move(data)); });
        if(!fs::exists(fs::path(m_currentLogFilePath))) {
            fs::create_directories(fs::path(m_currentLogFilePath).parent_path());
            std::ofstream log_file;
            log_file.open(m_currentLogFilePath);
            if(log_file.is_open()) {
                log_file.close();
            }
        }
    }

    void log_writer(std::string&& data) {
        if(m_currentFileSize + data.size() > m_maxFileSize) {
            if(m_prevLogFile != DEV_NULL) {
                fs::remove(m_prevLogFile);
                m_prevLogFile.clear();
            }

            m_prevLogFile = m_currentLogFilePath;
            m_currentLogFilePath = format_path(m_clearFilePath);
            m_currentFileSize = 0;
        }

        std::ofstream logFile;

        logFile.open(m_currentLogFilePath, std::fstream::app);
        if(logFile.is_open()) {
            logFile << data;
            m_currentFileSize += data.size();
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

    void set_max_size(uint64_t size) {
        this->m_currentFileSize = size;
    }

private:
    std::string get_timestamp(time_precision tp = time_precision::Ns) {
        auto now = chrono::system_clock::now();

        auto ms = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()) % 1000;
        auto mcs = chrono::duration_cast<chrono::microseconds>(now.time_since_epoch()) % 1000;
        auto ns = chrono::duration_cast<chrono::nanoseconds>(now.time_since_epoch()) % 1000;

        std::time_t t = std::time(nullptr);
        std::tm tm = *std::gmtime(&t);

        std::stringstream ss;
        ss << std::put_time(&tm, "%d.%m.%y_%H-%M-%S");

        if(tp >= time_precision::Ms) {
            ss << "." << std::setfill('0') << std::setw(3) << ms.count();
            if(tp >= time_precision::Mcs) {
                ss << "." << std::setfill('0') << std::setw(3) << mcs.count();
                if(tp >= time_precision::Mcs) {
                    ss << "." << std::setfill('0') << std::setw(3) << ns.count();
                }
            }
        }

        return ss.str();
    }

    std::string format_msg(const std::string& str) {
        return "[" + get_timestamp() + "] " + str + "\n";
    }

    std::string format_filename(const std::string& filename) {
        return fs::path(filename).filename().string() + "_" + get_timestamp(time_precision::Ms);
    }

    fs::path format_path(const fs::path& p) {
        return p.parent_path()/format_filename(p.filename().string());
    }

};
