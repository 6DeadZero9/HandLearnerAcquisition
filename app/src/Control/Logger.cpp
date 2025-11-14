#include <chrono>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <Logger.h>

namespace logger {
    Logger::~Logger() {
        if (m_log_file.is_open()) m_log_file.close();
    }

    void Logger::init(LogLevel log_level, const std::string& file_path) {
        std::lock_guard<std::mutex> lock(m_log_mutex);
        m_log_level = log_level;
        m_file_path = file_path;

        if (!m_file_path.empty()) {
            if (m_log_file.is_open()) m_log_file.close();

            m_log_file.open(m_file_path, std::ios::app);
        }
    }

    void Logger::log(LogLevel log_level, const std::string& file, int line, const std::string& message) {
        std::lock_guard<std::mutex> lock(m_log_mutex);
        if (log_level > m_log_level) return;

        auto now = std::chrono::system_clock::now();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        const std::string log_level_str = log_level_to_string(log_level);
        const std::string log_output =
            "[" + log_level_str + "] [" + get_timestamp() + "] " + file + ":" + std::to_string(line) + " " + message;
        std::cout << log_output << std::endl;

        write_to_file(log_output);
    }

    void Logger::write_to_file(const std::string& message) {
        if (!m_log_file.is_open()) return;

        m_log_file << message << std::endl;

        if (m_log_file.tellp() >= MAX_LOG_FILE_SIZE) {
            m_log_file.close();
            std::rename(m_file_path.c_str(), (m_file_path + ".old").c_str());
            m_log_file.open(m_file_path, std::ios::app);
        }
    }

    std::string Logger::get_timestamp() {
        std::time_t now = std::time(nullptr);
        std::tm* tm = std::localtime(&now);
        std::ostringstream oss;
        oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    std::string Logger::log_level_to_string(LogLevel log_level) {
        switch (log_level) {
            case LogLevel::LOG_EMERG:
                return "EMERGENCY";
            case LogLevel::LOG_ALERT:
                return "ALERT";
            case LogLevel::LOG_CRIT:
                return "CRITICAL";
            case LogLevel::LOG_ERR:
                return "ERROR";
            case LogLevel::LOG_WARNING:
                return "WARNING";
            case LogLevel::LOG_NOTICE:
                return "NOTICE";
            case LogLevel::LOG_INFO:
                return "INFO";
            case LogLevel::LOG_DEBUG:
                return "DEBUG";
            default:
                return "UNKNOWN";
        }
    }

} // namespace logger
