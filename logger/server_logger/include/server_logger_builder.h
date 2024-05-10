#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_SERVER_LOGGER_BUILDER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_SERVER_LOGGER_BUILDER_H

#include <logger_builder.h>
#include <map>
#include <set>
#include <unistd.h>
#include <sys/msg.h>
#include <queue>
#include "nlohmann/json.hpp"
#include <not_implemented.h>
#include <fstream>


#define LINUX_SERVER_KEY 100
#define MAX_TEXT_SIZE 10
#define SHUTDOWN_PRIOR 1

class server_logger_builder final:
    public logger_builder
{

private:
    struct msg_q
    {
        long m_type;
        pid_t pid;
        size_t packet_id;
        size_t packet_cnt;
        char file_path[256];
        int severity;
        char m_text[MAX_TEXT_SIZE];
    };

private:
    std::map<std::string, std::set<logger::severity>> _data;
private:
    void get_config_info(nlohmann::json &config, std::string const &configuration_path);
private:
    logger::severity get_severity(std::string const &severity_str);
private:
    void get_substr_queue(std::string const &str, std::queue<std::string> &queue, char separator);

public:

    server_logger_builder();

    server_logger_builder(
        server_logger_builder const &other) = default;

    server_logger_builder &operator=(
        server_logger_builder const &other) = default;

    server_logger_builder(
        server_logger_builder &&other) noexcept = default;

    server_logger_builder &operator=(
        server_logger_builder &&other) noexcept = default;

    ~server_logger_builder() noexcept override = default;

public:

    logger_builder *add_file_stream(
        std::string const &stream_file_path,
        logger::severity severity) override;

    logger_builder *add_console_stream(
        logger::severity severity) override;

    logger_builder* transform_with_configuration(
        std::string const &configuration_file_path,
        std::string const &configuration_path) override;

    logger_builder *clear() override;

    [[nodiscard]] logger *build() const override;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_SERVER_LOGGER_BUILDER_H