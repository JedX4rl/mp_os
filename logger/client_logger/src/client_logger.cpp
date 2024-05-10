#include "../include/client_logger.h"


std::unordered_map<std::string, std::pair<std::ofstream*, size_t>> client_logger::_all_streams;

client_logger::client_logger(std::map<std::string, std::set<logger::severity>> const &data,
                             std::string const &_log_structure)
{
    client_logger::_log_structure = _log_structure;
    for (auto iter = data.begin(); iter != data.end(); ++iter)
    {
        const auto& log_element = *iter;
        if (!client_logger::_all_streams.count(log_element.first))
        {
            if (!log_element.first[0])
            {
                client_logger::_all_streams[log_element.first] = std::pair<std::ofstream*, size_t> (nullptr, 0);
            }
            else
            {
                std::ofstream* file_stream;
                try
                {
                    file_stream = new ::std::ofstream(log_element.first);
                    if (!file_stream->is_open())
                    {
                        throw std::logic_error("File cannot be opened");
                    }
                }
                catch (std::exception &)
                {
                    for (auto curr_iter = data.begin(); curr_iter != iter; ++curr_iter)
                    {
                        decrement_streams(log_element.first);
                    }
                    throw std::exception();
                }
                client_logger::_all_streams[log_element.first] = std::pair<std::ofstream*, size_t>(file_stream, 0);
            }
        }
        client_logger::_all_streams[log_element.first].second++;
        _streams[log_element.first] = std::pair<std::ofstream*, std::set<logger::severity>>(
                client_logger::_all_streams[log_element.first].first, log_element.second);
    }
}

client_logger::client_logger(const client_logger &other):
    _log_structure(other._log_structure), _streams(other._streams)
{
    for (const auto& curr_stream: _streams)
    {
        _all_streams[curr_stream.first].second++;
    }
}

client_logger &client_logger::operator=(client_logger const &other)
{
    if (this == &other)
    {
        return *this;
    }
    for (const auto& curr_stream: _streams)
    {
        decrement_streams(curr_stream.first);
    }
    _log_structure = other._log_structure;
    _streams = other._streams;

    for (const auto& curr_stream: _streams)
    {
        _all_streams[curr_stream.first].second++;
    }
    return *this;
}

client_logger::client_logger(
        client_logger &&other) noexcept: //move constructor
        _log_structure(std::move(other._log_structure)),
        _streams(std::move(other._streams))
{

}

client_logger &client_logger::operator=(
        client_logger &&other) noexcept //move assignment
{
    if (this == &other)
    {
        return *this;
    }
    for (const auto& curr_stream : _streams)
    {
        decrement_streams(curr_stream.first);
    }
    _log_structure = std::move((other._log_structure));
    _streams = std::move(other._streams);

    return *this;
}

void client_logger::decrement_streams(std::string const &file_path) noexcept
{

    if (file_path.size() == 0)
    {
        return;
    }

    auto curr_iterator = _all_streams.find(file_path);
    if (curr_iterator == _all_streams.end())
    {
        return;
    }
    auto& stream = curr_iterator->second.first;
    auto& counter = curr_iterator->second.second;

    if (--(counter) == 0)
    {
        stream->flush();
        delete stream;
        _all_streams.erase(curr_iterator);
    }
}

client_logger::~client_logger() noexcept
{
    for (auto &log_stream: _streams)
    {
        decrement_streams(_all_streams.find(log_stream.first)->first);
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
                    string_to_return += current_date_to_string();
                    break;
                case 't':
                    string_to_return += current_time_to_string();
                    break;
                case 's':
                    string_to_return += severity_to_string(severity);
                    break;
                case 'm':
                    string_to_return += logger_msg;
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
            if (curr_stream.first[0])
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