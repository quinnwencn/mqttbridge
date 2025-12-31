#pragma once

#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <fmt/core.h>
#include <source_location>
#include <string_view>

#include "config.h"

class Logger {
public:
    static void Init(const MqttBridge::LogConfig& config);

    template <typename... Args>
    static void Trace(std::source_location loc, std::string_view fmt, Args&&... args) {
        log(boost::log::trivial::trace, fmt, loc, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void Info(std::source_location loc, std::string_view fmt, Args&&... args) {
        log(boost::log::trivial::info, fmt, loc, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void Debug(std::source_location loc, std::string_view fmt, Args&&... args) {
        log(boost::log::trivial::debug, fmt, loc, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void Warn(std::source_location loc, std::string_view fmt, Args&&... args) {
        log(boost::log::trivial::warning, fmt, loc, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void Error(std::source_location loc, std::string_view fmt, Args&&... args) {
        log(boost::log::trivial::error, fmt, loc, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void Fatal(std::source_location loc, std::string_view fmt, Args&&... args) {
        log(boost::log::trivial::fatal, fmt, loc, std::forward<Args>(args)...);
    }

private:
    static boost::log::sources::severity_logger<
        boost::log::trivial::severity_level>& Get();

    template <typename... Args>
    static void log(boost::log::trivial::severity_level lvl,
                    std::string_view fmt,
                    const std::source_location& loc,
                    Args&&... args) {

        std::string msg = fmt::format(
            fmt::runtime(fmt),
            std::forward<Args>(args)...);

        BOOST_LOG_SEV(Get(), lvl)
            << "[" << std::filesystem::path(loc.file_name()).filename().string() << ":" << loc.line() << "] "
            << msg;
    }
};

#define LOG_TRACE(fmt, ...) Logger::Trace(std::source_location::current(), fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) Logger::Info(std::source_location::current(), fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) Logger::Debug(std::source_location::current(), fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) Logger::Warn(std::source_location::current(), fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) Logger::Error(std::source_location::current(), fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) Logger::Fatal(std::source_location::current(), fmt, ##__VA_ARGS__)
