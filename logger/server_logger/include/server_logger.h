#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_SERVER_LOGGER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_SERVER_LOGGER_H

#include "server_logger_builder.h"
#include <logger.h>
#include <map>
#include <set>
#include <unistd.h>
#include <sys/msg.h>
#define LINUX_SERVER_KEY 100
#define MAX_TEXT_SIZE 10
#define MAIN_PRIOR 2

class server_logger final:
    public logger
{
    friend class server_logger_builder;

private:
    std::map<std::string, std::set<logger::severity>> _data;
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
    int mq_descriptor;

public:
    explicit server_logger(std::map<std::string, std::set<logger::severity>> const &_data);

    server_logger(
        server_logger const &other) = default;

    server_logger &operator=(
        server_logger const &other) = default;

    server_logger(
        server_logger &&other) noexcept = default;

    server_logger &operator=(
        server_logger &&other) noexcept = default;

    ~server_logger() noexcept final = default;

public:

    [[nodiscard]] logger const *log(
        const std::string &message,
        logger::severity severity) const noexcept override;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_SERVER_LOGGER_H