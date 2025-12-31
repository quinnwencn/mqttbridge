//
// Created by Quinn Wen on 2025/12/14.
//
#include "mqttbridge/logger.h"

#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace logging = boost::log;
namespace expr = boost::log::expressions;

boost::log::sources::severity_logger<
    boost::log::trivial::severity_level>& Logger::Get() {
    static boost::log::sources::severity_logger<
        boost::log::trivial::severity_level> lg;
    return lg;
}

void Logger::Init(const MqttBridge::LogConfig& config) {
    const auto& logDir = config.LogDir();
    std::filesystem::create_directories(logDir);
    auto logFile = logDir / "mqttbridge.log";

    logging::add_common_attributes();
    logging::add_file_log(
        logging::keywords::file_name = logFile.string(),
        logging::keywords::rotation_size = 100 * 1024 * 1024, // 100MB
        logging::keywords::auto_flush = true,
        logging::keywords::format =
            (
                expr::stream
                    << "[" << expr::format_date_time<boost::posix_time::ptime>(
                           "TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
                    << "] "
                    << "[" << logging::trivial::severity << "] "
                    << expr::smessage
            )
    );

    logging::core::get()->set_filter(
        logging::trivial::severity >=
        static_cast<boost::log::trivial::severity_level>(config.LogLevel())
    );
}
