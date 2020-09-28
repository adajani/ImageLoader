#pragma once
#include <ctime>
#include <string>
#include <fstream>

#define LOG_INFO(msg) \
    Logger::GetInstance().Log(__FUNCDNAME__, msg, LoggerLevel::INFO);

#define LOG_DEBUG(msg) \
    Logger::GetInstance().Log(__FUNCDNAME__, msg, LoggerLevel::DEBUG);

#define LOG_WARN(msg) \
    Logger::GetInstance().Log(__FUNCDNAME__, msg, LoggerLevel::WARN);

#define LOG_ERROR(msg) \
    Logger::GetInstance().Log(__FUNCDNAME__, msg, LoggerLevel::ERROR);

const std::string LoggerFile = "ImageLoader.log";

enum LoggerLevel {
    INFO,
    WARN,
    DEBUG,
    ERROR,
    SILENT,
    MAX_LEVEL
};
#include <iostream>
class Logger {
public:
    static Logger& GetInstance();
    static void DestroyInstance();
    virtual void SetLoggerFile(std::string fileName);
    virtual void Log(const char* const location, const char* const data, LoggerLevel level);

    Logger& operator<< (const LoggerLevel &level);
    Logger& operator<< (const char *data);

private:
    Logger();
    ~Logger();

    static Logger* m_pInstance;
    static const char* const m_pLogLevels[LoggerLevel::MAX_LEVEL];
    std::ofstream m_outStream;
    LoggerLevel m_LoggerLevel;
    std::string m_LoggerFilename = LoggerFile;
    const std::string getCurrentDateTime();
    void formatter(const char * const location, const char * const data, LoggerLevel level);
};
