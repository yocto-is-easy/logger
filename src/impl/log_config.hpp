#pragma once

#include <string>
#include <filesystem>

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace fs = std::filesystem;
namespace ptree = boost::property_tree;

class log_config
{
private:
    ptree::ptree m_config;
    fs::path m_configPath;

public:
    log_config(const fs::path& confPath)
        : m_configPath(confPath) {}

    log_config(log_config&&) = delete;

    bool load() {
        if(!fs::exists(m_configPath)) {
            fs::create_directories(m_configPath.parent_path());
            return false;
        }

        ptree::read_ini(m_configPath.string(), m_config);
        return true;
    }

    void save() {
        fs::create_directories(m_configPath.parent_path());
        ptree::write_ini(m_configPath, m_config);
    }

    template<typename T>
    T get_field(const std::string& field) {
        return m_config.get<T>(field);
    }

    void set_field(const std::string& field, const std::string& value) {
        m_config.put(field, value);
    }

    void set_field(const std::string& field, uint64_t value) {
        m_config.put(field, std::to_string(value));
    }
};
