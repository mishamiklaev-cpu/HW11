#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <fstream>

namespace Engine
{
    enum class LogLevel
    {
        INFO,
        WARNING,
        ERROR
    };

    class LogSink
    {
    public:
        virtual void log(LogLevel level, const std::string& message) = 0;
        virtual ~LogSink() = default;
    };

    class ConsoleSink : public LogSink
    {
    public:
        void log(LogLevel level, const std::string& message) override;
    private:
        std::string logLevelToString(LogLevel level);
    };

    class FileSink : public LogSink
    {
    public:
        FileSink(const std::string& filename);
        ~FileSink();
        void log(LogLevel level, const std::string& message) override;
    private:
        std::ofstream logFile;
        std::string logLevelToString(LogLevel level);
    };

    class Logger
    {
    public:
        void addSink(std::shared_ptr<LogSink> sink);
        void log(LogLevel level, const std::string& message);
        void info(const std::string& message);
        void warn(const std::string& message);
        void error(const std::string& message);
    private:
        std::vector<std::shared_ptr<LogSink>> sinks;
        std::mutex logMutex;
    };

    class LoggerRegistry
    {
    public:
        static LoggerRegistry& getInstance();
        std::shared_ptr<Logger> getLogger(const std::string& name);
        void setDefaultLogger(std::shared_ptr<Logger> logger);
        void registerLogger(const std::string& name, std::shared_ptr<Logger> logger);
    private:
        LoggerRegistry() = default;
        std::unordered_map<std::string, std::shared_ptr<Logger>> loggers;
        std::shared_ptr<Logger> defaultLogger;
        std::mutex registryMutex;
    };

    void setupLogger();
}

#define LOG_INFO(msg) ::Engine::LoggerRegistry::getInstance().getLogger("global")->info(msg)
#define LOG_WARN(msg) ::Engine::LoggerRegistry::getInstance().getLogger("global")->warn(msg)
#define LOG_ERROR(msg) ::Engine::LoggerRegistry::getInstance().getLogger("global")->error(msg)