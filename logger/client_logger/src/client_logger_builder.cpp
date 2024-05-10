#include <not_implemented.h>
#include "../include/client_logger.h"


client_logger_builder::client_logger_builder() : _log_structure("%d %t %s %m")
{

}

logger_builder* client_logger_builder::add_file_stream(std::string const &stream_file_path, logger::severity severity)
{
    if (stream_file_path.empty())
    {
        throw std::invalid_argument("Invalid arg");
    }
    std::filesystem::path file(stream_file_path);
    std::filesystem::path absolute_path = std::filesystem::absolute(file);
    _data[absolute_path].insert(severity);
    return this;
}

logger_builder* client_logger_builder::add_console_stream(
        logger::severity severity)
{
    _data[""].insert(severity);
    return this;
}

logger::severity client_logger_builder::get_severity(std::string const &severity_str)
{
    switch (severity_str[0])
    {
        case 'd':
            return logger::severity::debug;
        case 't':
            return logger::severity::trace;
        case 'i':
            return logger::severity::information;
        case 'w':
            return logger::severity::warning;
        case 'c':
            return logger::severity::critical;
        case 'e':
            return logger::severity::error;
        default:
            return logger::severity::information;
    }
}

logger_builder* client_logger_builder::transform_with_configuration(std::string const &configuration_file_path, std::string const &configuration_path)
{

    std::ifstream configuration_file(configuration_file_path);

    if (!configuration_file.is_open())
    {
        throw std::runtime_error("File cannot be opened");
    }
    nlohmann::json config = nlohmann::json::parse(configuration_file);

    get_config_info(config, configuration_path);

    configuration_file.close();

    return this;
}

void client_logger_builder::get_config_info(nlohmann::json &config, std::string const &configuration_path)
{
    std::string buf;
    std::string severity_str;
    logger::severity severity;

    std::queue<std::string> queue;

    get_substr_queue(configuration_path, queue, '/');

    while (!queue.empty())
    {
        buf = queue.front();
        queue.pop();

        if (config.find(buf) != config.end())
        {
            config = config[buf];
        }
        else
        {
            throw std::runtime_error("Configuration was not found");
        }
    }

    _log_structure = config["format"];

    config = config["paths"];

    for (auto& file : config.items())
    {
        buf = file.key();

        for (auto& severity_iter : file.value())
        {
            severity_str = severity_iter;
            severity = get_severity(severity_str);

            if (buf[0] == '\0')
            {
                add_console_stream(severity);
            }
            else
            {
                add_file_stream(buf, severity);
            }
        }
    }
}

void client_logger_builder::get_substr_queue(std::string const &str, std::queue<std::string> &queue, char separator)
{
    std::string substr;
    size_t i = 0, j = 0;

    while (str[i] != '\0')
    {
        if (str[i] == separator)
        {
            queue.push(substr);
            j = 0;
            substr.clear();
        }
        else
        {
            substr.replace(j, 1, 1, str[i]);
            ++j;
        }

        ++i;
    }

    queue.push(substr);
}

logger_builder* client_logger_builder::clear()
{
    _data.clear();
    return this;
}

client_logger_builder::client_logger_builder(const std::string &info)
{
    if (!info.empty())
    {
        _log_structure = info;
    }
}

logger* client_logger_builder::build() const
{
    return new client_logger(_data, _log_structure);
}