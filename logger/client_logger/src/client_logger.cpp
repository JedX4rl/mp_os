#include <not_implemented.h>

#include "../include/client_logger.h"

client_logger::client_logger(
    client_logger const &other)
{
    for (const auto &stream: other._streams)
    {
        std::string key = stream.first;
        std::pair<std::ofstream *, std::set<logger::severity>> value;

        value.first = stream.second.first;
        value.second = stream.second.second;
        _streams[key] = value;
        _all_streams[key].second++;
    }
}

client_logger::client_logger(const std::map<std::string, std::set<logger::severity>>& data)
{
    for (const auto &part: data)
    {
        if (!_all_streams.count(part.first)) //TODO TRY CATCH
        {
            auto* file_stream = new std::ofstream(part.first);
            _all_streams[part.first] = std::pair<std::ofstream*, size_t>(file_stream, 0);
        }
        _all_streams[part.first].second++;
        _streams[part.first] = std::pair<std::ofstream*, std::set<logger::severity>>(_all_streams[part.first].first, part.second);
    }

    throw not_implemented("client_logger &client_logger::operator=(client_logger const &other)", "your code should be here...");
}

client_logger &client_logger::operator=(
    client_logger const &other)
{
    throw not_implemented("client_logger &client_logger::operator=(client_logger const &other)", "your code should be here...");
}

client_logger::client_logger(
    client_logger &&other) noexcept
{
    throw not_implemented("client_logger::client_logger(client_logger &&other) noexcept", "your code should be here...");
}

client_logger &client_logger::operator=(
    client_logger &&other) noexcept
{
    throw not_implemented("client_logger &client_logger::operator=(client_logger &&other) noexcept", "your code should be here...");
}

client_logger::~client_logger() noexcept
{
    throw not_implemented("client_logger &client_logger::operator=(client_logger &&other) noexcept", "your code should be here...");
}

logger const *client_logger::log(
    const std::string &text,
    logger::severity severity) const noexcept
{
    throw not_implemented("logger const *client_logger::log(const std::string &text, logger::severity severity) const noexcept", "your code should be here...");
}