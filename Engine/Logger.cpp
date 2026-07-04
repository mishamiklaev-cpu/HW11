#include "pch.h"
#include "Logger.h"

namespace Engine
{
    std::string ConsoleSink::logLevelToString(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::INFO: return "[INFO]";
        case LogLevel::WARNING: return "[WARNING]";
        case LogLevel::ERROR: return "[ERROR]";
        default: return "[UNKNOWN]";
        }
    }

    void ConsoleSink::log(LogLevel level, const std::string& message)
    {
        std::cout << logLevelToString(level) << " " << message << std::endl;
    }

    FileSink::FileSink(const std::string& filename)
    {
        logFile.open(filename, std::ios::app);
    }

    FileSink::~FileSink()
    {
        if (logFile.is_open()) logFile.close();
    }

    std::string FileSink::logLevelToString(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::INFO: return "[INFO]";
        case LogLevel::WARNING: return "[WARNING]";
        case LogLevel::ERROR: return "[ERROR]";
        default: return "[UNKNOWN]";
        }
    }

    void FileSink::log(LogLevel level, const std::string& message)
    {
        if (logFile)
        {
            logFile << logLevelToString(level) << " " << message << std::endl;
        }
    }

    void Logger::addSink(std::shared_ptr<LogSink> sink)
    {
        sinks.push_back(sink);
    }

    void Logger::log(LogLevel level, const std::string& message)
    {
        std::lock_guard<std::mutex> lock(logMutex);
        for (auto& sink : sinks)
        {
            sink->log(level, message);
        }
    }

    void Logger::info(const std::string& message) { log(LogLevel::INFO, message); }
    void Logger::warn(const std::string& message) { log(LogLevel::WARNING, message); }
    void Logger::error(const std::string& message) { log(LogLevel::ERROR, message); }

    LoggerRegistry& LoggerRegistry::getInstance()
    {
        static LoggerRegistry instance;
        return instance;
    }

    std::shared_ptr<Logger> LoggerRegistry::getLogger(const std::string& name)
    {
        std::lock_guard<std::mutex> lock(registryMutex);
        if (loggers.find(name) != loggers.end())
        {
            return loggers[name];
        }
        return defaultLogger;
    }

    void LoggerRegistry::setDefaultLogger(std::shared_ptr<Logger> logger)
    {
        defaultLogger = logger;
    }

    void LoggerRegistry::registerLogger(const std::string& name, std::shared_ptr<Logger> logger)
    {
        std::lock_guard<std::mutex> lock(registryMutex);
        loggers[name] = logger;
    }

    void setupLogger()
    {
        auto logger = std::make_shared<Logger>();
        logger->addSink(std::make_shared<ConsoleSink>());
        logger->addSink(std::make_shared<FileSink>("log.txt"));
        LoggerRegistry::getInstance().registerLogger("global", logger);
        LoggerRegistry::getInstance().setDefaultLogger(logger);
    }
}