#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_BUILDER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_BUILDER_H

#include <logger_builder.h>
#include <map>
#include <set>
#include <queue>
#include "nlohmann/json.hpp"

class client_logger_builder final:
    public logger_builder
{

private:
    std::map<std::string, std::set<logger::severity>> _data;
private:
    std::string _log_structure;
private:
    void get_config_info(nlohmann::json &config, std::string const &configuration_path);
private:
    static void get_substr_queue(std::string const &str, std::queue<std::string> &queue, char separator);
private:
    static logger::severity get_severity(std::string const &severity_str);
public:

    explicit client_logger_builder (std::string const & info);

    client_logger_builder();

    client_logger_builder(
        client_logger_builder const &other) = delete;

    client_logger_builder &operator=(
        client_logger_builder const &other) = delete;

    client_logger_builder(
        client_logger_builder &&other) noexcept = delete;

    client_logger_builder &operator=(
        client_logger_builder &&other) noexcept = delete;

    ~client_logger_builder() noexcept override = default;

public:

    logger_builder *add_file_stream(
        std::string const &stream_file_path,
        logger::severity severity) override;

    logger_builder *add_console_stream(
        logger::severity severity) override;

    logger_builder *add_file_stream(
        std::string const &stream_file_path,
        std::string const &severity);

    logger_builder *add_console_stream(
            std::string const &severity);

    logger_builder* transform_with_configuration(
        std::string const &configuration_file_path,
        std::string const &configuration_path) override;

    logger_builder *clear() override;


    [[nodiscard]] logger *build() const override;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_BUILDER_H