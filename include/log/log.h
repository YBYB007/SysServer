#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <mutex>
#include <ctime>

#define LOG(level, ...) ServerLogger::getInstance().log(level, __FILE__, __LINE__, __VA_ARGS__)

enum class LogLevel
{
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class ServerLogger
{
public:
    static ServerLogger &getInstance()
    {
        static ServerLogger instance("../build.log"); // 静态局部变量确保只初始化一次
        return instance;
    }

    void setLogLevel(LogLevel level)
    {
        logLevel_ = level;
    }

    template <typename... Args>
    void log(LogLevel level, const std::string &file, int line, const Args &...args)
    {
        if (level >= logLevel_)
        {
            std::ostringstream logStream;
            logStream << getCurrentTime() << " ";
            logStream << "[" << logLevelToString(level) << "] ";
            logStream << "[" << file << ":" << line << "] ";
            logStream << formatMessage(args...) << "\
            ";

            std::lock_guard<std::mutex> lock(mutex_);
            logFile_ << logStream.str();
            logFile_.flush();
        }
    }

private:
    ServerLogger(const std::string &filename)
        : logFile_(filename), logLevel_(LogLevel::INFO) {}

    std::ofstream logFile_;
    LogLevel logLevel_;
    std::mutex mutex_;

    std::string getCurrentTime()
    {
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::tm tm_now = *std::localtime(&now_c);
        std::ostringstream oss;
        oss << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    std::string logLevelToString(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARNING:
            return "WARNING";
        case LogLevel::ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
        }
    }

    template <typename T>
    std::string formatMessage(const T &message)
    {
        std::ostringstream oss;
        oss << message;
        return oss.str();
    }

    template <typename T, typename... Args>
    std::string formatMessage(const T &message, const Args &...args)
    {
        std::ostringstream oss;
        oss << message << " ";
        oss << formatMessage(args...);
        return oss.str();
    }
};
