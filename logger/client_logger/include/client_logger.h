#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H

#include <logger.h>
#include "client_logger_builder.h"
#include <not_implemented.h>
#include <map>
#include <unordered_map>
#include <set>
#include <string>
#include <fstream>

class client_logger final:
    public logger
{
    friend class client_logger_builder;

private:
    std::map<std::string, std::pair<std::ofstream *, std::set<logger::severity>>> _streams;
private:
    static std::unordered_map<std::string, std::pair<std::ofstream *, size_t>> _all_streams;
private:
    std::string _log_structure;



private:
    explicit client_logger(std::map<std::string, std::set<logger::severity>> const& data,
                           std::string const &_log_structure);
    
private:
    [[nodiscard]] std::string parse_string(
            std::string const &logger_msg, logger::severity severity) const;
    static void decrement_streams(std::string const &file_path) noexcept;

public:

    client_logger(
        client_logger const &other); //TODO Rule of five

    client_logger &operator=(
        client_logger const &other);

    client_logger(
        client_logger &&other) noexcept;

    client_logger &operator=(
        client_logger &&other) noexcept;

    ~client_logger() noexcept final;

public:

    [[nodiscard]] logger const *log(
        const std::string &message,
        logger::severity severity) const noexcept override;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H