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


static std::unique_ptr<spdlog::logger> SHTReaderLogger_ptr;
static std::once_flag initFlag;

static void initSHTReaderLogger(){
    spdlog::sink_ptr console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::debug);

    spdlog::sink_ptr file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("/home/root/SHTReader.log", true);
    file_sink->set_level(spdlog::level::debug);

    spdlog::sinks_init_list sink_list = { file_sink, console_sink };
        
    spdlog::logger logger("SHTReaderLogger", sink_list.begin(), sink_list.end());

    logger.set_level(spdlog::level::debug);

    SHTReaderLogger_ptr = std::make_unique<spdlog::logger>(logger);
}

static const std::unique_ptr<spdlog::logger>& SHTLogger(){
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
    loggerInterface();
    virtual const std::shared_ptr<spdlog::async_logger>& logger() = 0;
    virtual ~loggerInterface();
};

class TemperatureReaderLogger : loggerInterface{
    static std::shared_ptr<spdlog::async_logger> _logger;
    static std::once_flag _initFlag;
    static std::shared_ptr<spdlog::details::thread_pool> _threadPool;
public:
    TemperatureReaderLogger();
    const std::shared_ptr<spdlog::async_logger>& logger() override;
private:
    static void initLogger();
};



#endif //SHTLOGGER_H

