#include <not_implemented.h>
#include "../include/client_logger.h"
#include <exception>
#include <filesystem>
#include <sstream>


#include "../include/client_logger_builder.h"


client_logger_builder::client_logger_builder()
{
    _log_structure = "%d %t %s %m";
}

logger_builder* client_logger_builder::add_file_stream(
        std::string const &stream_file_path,
        logger::severity severity)
{
    if (stream_file_path.empty())
    {
        throw std::invalid_argument("Invalid arg");;
    }
    std::filesystem::path file(stream_file_path);
    std::filesystem::path absolute_path = std::filesystem::absolute(file);
    _data[absolute_path].insert(severity);
    return this;
}

logger_builder* client_logger_builder::add_console_stream(
        logger::severity severity)
{
    _data["console"].insert(severity);
    return this;
}

logger::severity get_severity(char c)
{
    switch (c)
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

logger_builder* client_logger_builder::add_file_stream(
        std::string const &stream_file_path,
        std::string const &severity)
{
    if (stream_file_path.empty())
    {
        throw std::invalid_argument("Invalid arg");;
    }
    std::filesystem::path file(stream_file_path);
    std::filesystem::path absolute_path = std::filesystem::absolute(file);
    for (char c: severity)
    {
        _data[absolute_path].insert(get_severity(c));
    }
    return this;
}

logger_builder* client_logger_builder::add_console_stream(std::string const &severity)
{
    for (char c: severity)
    {
        _data["console"].insert(get_severity(c));
    }
    return this;
}



std::queue<std::string> client_logger_builder::configuration_path_to_substr(std::string const &conf_path)
{
    std::queue<std::string> result;
    std::istringstream sin(conf_path);
    std::string path_part;
    while (getline(sin, path_part, '/'))
    {
        result.push(path_part);
    }
    return result;
}

std::pair<std::string, std::vector<std::string>>
get_config_info(std::ifstream &configuration_file, std::queue<std::string> &path_queue)
{

    std::string prev_str, str;
    while (!configuration_file.eof() && !path_queue.empty())
    {
        std::getline(configuration_file >> std::ws, str);
        if (str == "{")
        {
            if (prev_str == path_queue.front())
            {
                path_queue.pop();
            }
        }
        prev_str = str;
    }
    std::string log_format;
    std::getline(configuration_file >> std::ws, log_format, '\n');
    std::vector<std::string> file_vector;
    str = "";

    while (!configuration_file.eof() && str != "}")
    {
        getline(configuration_file >> std::ws, str, '\n');
        if (str != "}")
        {
            file_vector.push_back(str);
        }
    }
    if (configuration_file.eof())
    {
        throw std::invalid_argument("Nesting problems");
    }
    return std::make_pair(log_format, file_vector);
}

std::vector<std::string> split_string(const std::string &str, char delim)
{
    std::vector<std::string> result;
    std::string current;
    bool inside_quotes = false;

    for (char c: str)
    {
        if (c == '\"')
        {
            current += '"';
            inside_quotes = !inside_quotes;
        }
        else if (c == delim && !inside_quotes)
        {
            result.push_back(current);
            current.clear();
        }
        else
        {
            current += c;
        }
    }

    result.push_back(current);
    return result;
}


logger_builder* client_logger_builder::transform_with_configuration(
        std::string const &configuration_file_path,
        std::string const &configuration_path)
{
    std::string prev_str, str;
    std::ifstream configuration_file;
    configuration_file.open(configuration_file_path);
    if (!configuration_file.is_open())
    {
        throw std::runtime_error("File cannot be opened\n");
    }
    std::queue<std::string> path_queue = configuration_path_to_substr(configuration_path);
    std::pair<std::string, std::vector<std::string>> logger_settings =
            get_config_info(configuration_file, path_queue);
    change_log_structure(logger_settings.first);
    for (auto &curr_stream: logger_settings.second)
    {
        std::vector<std::string> settings_vector = split_string(curr_stream, ':');
        std::string name = settings_vector.front();

        if (name[0] == '"')
        {
            add_file_stream(name, settings_vector[1]);
        }
        else
        {
            add_console_stream(settings_vector[1]);
        }
    }
    return this;

}

logger_builder* client_logger_builder::clear()
{
    _data.clear();
    return this;
}

logger_builder* client_logger_builder::change_log_structure(const std::string &new_structure)
{
    if (new_structure.empty())
    {
        throw std::invalid_argument("Structure cannot be empty\n");
    }
    _log_structure = new_structure == "default" ? "%d %t %s %m" : new_structure;
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
    return new client_logger(_data, _log_structure);;
}