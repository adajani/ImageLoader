#include "stdafx.h"
#include "Logger.h"
#include <iostream>

#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

Logger* Logger::m_pInstance = nullptr;

const char* const Logger::m_pLogLevels[LoggerLevel::MAX_LEVEL] \
{"INFO", "WARN", "DEBUG", "ERROR", "SILENT"};

Logger & Logger::GetInstance()
{
    if (m_pInstance == nullptr) {
        m_pInstance = new Logger();
    }
    return *m_pInstance;
}

Logger::Logger() {
    m_outStream.open(m_LoggerFilename);
    if (!m_outStream.is_open()) {
        throw "Can't create logger file";
    }
}

Logger::~Logger(){
    m_outStream.close();
}

void Logger::DestroyInstance()
{
    delete m_pInstance;
    m_pInstance = nullptr;
}

void Logger::SetLoggerFile(std::string fileName)
{
    m_LoggerFilename = fileName;
    if (m_outStream.is_open()) {
        m_outStream.close();
    }
    m_outStream.open(m_LoggerFilename);
}

const std::string Logger::getCurrentDateTime() {
    static char buf[80] = { 0 };
    struct tm  tstruct;
    time_t  now = time(NULL);
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

void Logger::formatter(const char * const location, const char * const data, LoggerLevel level)
{
    m_outStream << getCurrentDateTime() << "," <<
        m_pLogLevels[level] << "," <<
        location << "," <<
        data << std::endl;
    m_outStream.flush();
}

void Logger::Log(const char * const location, const char * const data, LoggerLevel level)
{
    if (level >= m_LoggerLevel) {
        formatter(location, data, level);
    }
}

Logger& Logger::operator<< (const LoggerLevel &level) {
    m_LoggerLevel = level;
    m_outStream << getCurrentDateTime() << "," <<
        m_pLogLevels[m_LoggerLevel];
    return *this;
}

Logger& Logger::operator<< (const char *data) {
    m_outStream << "," << data;
    m_outStream.flush();
    return *this;
}
