#include "../headers/logger.hpp"
#include "logger.hpp"

loggerInterface::loggerInterface(std::string &logFilePath, 
                                logLevel level, 
                                int maxFileSize, 
                                int maxFiles, 
                                bool logToConsole, 
                                std::function<void(const spdlog::details::log_msg &msg)> callBack)
{
    _logFilepath = logFilePath;
    _logLevel = level;
    _maxFileSize = _maxFileSize;
    _logToConsole = logToConsole;
    _callBack = callBack;
}