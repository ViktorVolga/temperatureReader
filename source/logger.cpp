#include "../headers/logger.hpp"

loggerInterface::loggerInterface()
{

}

loggerInterface::~loggerInterface()
{

}

std::shared_ptr<spdlog::details::thread_pool> TemperatureReaderLogger::_threadPool;
std::shared_ptr<spdlog::async_logger> TemperatureReaderLogger::_logger;
std::once_flag TemperatureReaderLogger::_initFlag;

TemperatureReaderLogger::TemperatureReaderLogger() : loggerInterface()
{
    std::call_once(initFlag, initLogger);
}

const std::shared_ptr<spdlog::async_logger> &TemperatureReaderLogger::logger()
{
    return _logger;
}

void TemperatureReaderLogger::initLogger()
{
    _threadPool = std::make_shared<spdlog::details::thread_pool>(128, 2);
    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt >();
    auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("/home/root/SHTReader.log", 1024*1024*10, 3);
    rotating_sink->set_level(spdlog::level::debug);
    stdout_sink->set_level(spdlog::level::debug);
    spdlog::sinks_init_list sink_list = { rotating_sink, stdout_sink };
    _logger = std::make_shared<spdlog::async_logger>("Temperature reader", sink_list.begin(), sink_list.end(), _threadPool, spdlog::async_overflow_policy::block);
}