#ifndef LOGGER_H_
#define LOGGER_H_

#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <mutex>
#include <string>

#include <Singleton.h>
#include <Tools.h>

constexpr int32_t MAX_LOG_FILE_SIZE { 1024 * 512 };

namespace logger {
    enum LogLevel {
        LOG_EMERG,
        LOG_ALERT,
        LOG_CRIT,
        LOG_ERR,
        LOG_WARNING,
        LOG_NOTICE,
        LOG_INFO,
        LOG_DEBUG,
    };

    /**
     * @brief System wide logger. Logs to file and stdout
     */
    class Logger : public Singleton<Logger> {
        friend class Singleton<Logger>;

    public:
        ~Logger();

        /**
         * @brief Logger setup with max log level, path to logging
         * file and boolean that decides whether or not to log to
         * syslog
         * @param log_level max log level which should be logged
         * @param file_path self-explanatory - path to logging file
         * @param syslog bool that determines whether or not to log to
         * syslog
         */
        void init(LogLevel log_level = LOG_INFO, const std::string& file_path = "");

        /**
         * @brief Logging function that initiates the logging pipeline,
         * thread safe implementation
         * @param log_level log level for current message
         * @param file name of the file from which the log function was called
         * @param line line number at which the logging was initiated
         * syslog
         * @param message printf style unformatted message that will be emplaced
         * with provided variadic arguments
         */
        void log(LogLevel log_level, const std::string& file, int line, const std::string& message);

    private:
        /**
         * @brief Function for string representation of the log level
         * @param log_level log level of interest
         * @return string representation of provided log level
         */
        std::string log_level_to_string(LogLevel log_level);

        /**
         * @brief Get current timestamp for log formatting
         * @return Formatted string representation of current timestamp
         */
        std::string get_timestamp();

        /**
         * @brief Function that writes the formatted message to a given file.
         * Skipped if the file is not open (file name not provided or file wasn't
         * open)
         * @param message Formatted message
         */
        void write_to_file(const std::string& message);

        std::ofstream m_log_file;
        std::mutex m_log_mutex;
        std::string m_file_path;
        LogLevel m_log_level { LogLevel::LOG_INFO };
    };

} // namespace logger

#define LOGGER_INSTANCE logger::Logger::instance()

#if defined(NO_LOGS)
#define LOG(log_level, message, ...)
#define LOG_NO_SERVER(log_level, message, ...)
#else
#define LOG(log_level, message, ...) LOGGER_INSTANCE.log(log_level, __FILE__, __LINE__, tools::format_string(message, ##__VA_ARGS__))
#define LOG_NO_SERVER(log_level, message, ...)                                                                                             \
    LOGGER_INSTANCE.log(log_level, __FILE__, __LINE__, tools::format_string(message, ##__VA_ARGS__), false)
#endif
#endif // LOGGER_H_