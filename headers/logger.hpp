#ifndef SHTLOGGER_H
#define SHTLOGGER_H
#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/async.h" //support for async logging.
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include <memory>
#include "spdlog/cfg/env.h"
#include <functional>

#include <mutex>

/*
    todo list
    1. logger on sturt up read config. And depends on configuration set up synk levels    
*/

enum class logLevel : int {
    trace = SPDLOG_LEVEL_TRACE,
    debug = SPDLOG_LEVEL_DEBUG,
    info = SPDLOG_LEVEL_INFO,
    warn = SPDLOG_LEVEL_WARN,
    err = SPDLOG_LEVEL_ERROR,
    critical = SPDLOG_LEVEL_CRITICAL,
    off = SPDLOG_LEVEL_OFF,
    n_levels
};


static std::shared_ptr<spdlog::async_logger> SHTReaderLogger_ptr;
static std::once_flag initFlag;

static void initSHTReaderLogger(){
    spdlog::init_thread_pool(8192, 1);
    spdlog::sink_ptr console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::debug);

    spdlog::sink_ptr file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("/home/root/SHTReader.log", 1024*1024*10, 3);
    file_sink->set_level(spdlog::level::debug);

    spdlog::sinks_init_list sink_list = { file_sink, console_sink }; 
        
    //spdlog::logger logger("SHTReaderLogger", sink_list.begin(), sink_list.end());
    SHTReaderLogger_ptr = std::make_shared<spdlog::async_logger>("SHTReaderLogger", sink_list.begin(), sink_list.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);

    SHTReaderLogger_ptr->set_level(spdlog::level::debug);

    
}

static const std::shared_ptr<spdlog::async_logger>& SHTLogger(){
    std::call_once(initFlag, initSHTReaderLogger);
    return SHTReaderLogger_ptr;
}

class loggerInterface{    
    logLevel _logLevel;
    spdlog::sink_ptr _consoleSink;
    spdlog::sink_ptr _fileSink;
    spdlog::sink_ptr _callbackSink;
    bool _logToConsole = false;
    int _maxFileSize {1048576 * 5};
    int _maxFiles {10};
    std::string _logFilepath;
    std::function<void(const spdlog::details::log_msg &msg)> _callBack;
public:
    loggerInterface(std::string & logFilePath,
                    logLevel level,
                    int maxFileSize = 1048576 * 5,
                    int maxFiles = 10,
                    bool logToConsole = false,
                    std::function<void(const spdlog::details::log_msg &msg)> callBack = nullptr);
    virtual const std::unique_ptr<spdlog::async_logger>& logger() = 0;
    virtual ~loggerInterface();    
};

class TemperatureReaderLogger : loggerInterface{
    static std::unique_ptr<spdlog::async_logger> _logger;
    static std::once_flag _initFlag;
public:
    const std::unique_ptr<spdlog::async_logger>& logger() override;
};



#endif //SHTLOGGER_H

