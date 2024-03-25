#include <not_implemented.h>
#include <map>

#include "../include/client_logger.h"


std::unordered_map<std::string, std::pair<std::ofstream*, size_t>> client_logger::_all_streams;

client_logger::client_logger(std::map<std::string, std::set<logger::severity>> const &data,
                             std::string const &_log_structure)
{
    client_logger::_log_structure = _log_structure;
    for (const auto &log_element: data)
    {
        if (!client_logger::_all_streams.count(log_element.first))
        {
            if (log_element.first == "console")
            {
                client_logger::_all_streams[log_element.first] = std::pair<std::ofstream*, size_t> (nullptr, 0);
            }
            else
            {
                auto* file_stream = new ::std::ofstream(log_element.first);
                client_logger::_all_streams[log_element.first] = std::pair<std::ofstream*, size_t>(file_stream, 0);
            }
        }
        client_logger::_all_streams[log_element.first].second++;
        _streams[log_element.first] = std::pair<std::ofstream*, std::set<logger::severity>>(
                client_logger::_all_streams[log_element.first].first, log_element.second);
    }
}

client_logger::~client_logger() noexcept
{
    for (auto &log_stream: _streams)
    {
        auto global_stream = _all_streams.find(log_stream.first);
        if (--(global_stream->second.second) == 0)
        {
            if (global_stream->second.first != nullptr)
            {
                global_stream->second.first->flush();
                global_stream->second.first->close();
                delete global_stream->second.first;

            }
            _all_streams.erase(global_stream);
        }
    }
}

std::string client_logger::parse_string(const std::string &logger_msg, logger::severity severity) const
{
    std::string string_to_return;
    size_t log_structure_size = _log_structure.size();
    for (int i = 0; i < log_structure_size; ++i)
    {
        if (_log_structure[i] == '%' && (i != (log_structure_size - 1)))
        {
            switch (_log_structure[i + 1])
            {
                case 'd':
                    string_to_return += '[' + current_date_to_string() + "] ";
                    break;
                case 't':
                    string_to_return += '[' + current_time_to_string() + "] ";
                    break;
                case 's':
                    string_to_return += '(' + severity_to_string(severity) + ") ";
                    break;
                case 'm':
                    string_to_return += logger_msg + ' ';
                    break;
                default:
                    string_to_return += '%';
                    string_to_return += _log_structure[i + 1];
                    break;
            }
            i++;
        }
        else
        {
            string_to_return += _log_structure[i];
        }
    }
    return string_to_return;
}

logger const* client_logger::log(
        const std::string &text,
        logger::severity severity) const noexcept
{

    std::string out_string = parse_string(text, severity);
    for (auto &curr_stream: _streams)
    {
        if (curr_stream.second.second.find(severity) != curr_stream.second.second.end())
        {
            if (curr_stream.first != "console")
            {
                *curr_stream.second.first << out_string << std::endl;
                curr_stream.second.first->flush();
            }
            else
            {
                std::cout << out_string << std::endl;
            }
        }
    }
    return this;
}