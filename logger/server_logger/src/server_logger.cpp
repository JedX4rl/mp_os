#include <not_implemented.h>

#include "../include/server_logger.h"


server_logger::server_logger(std::map<std::string, std::set<logger::severity>> const &data):
    _data(data)
{
    mq_descriptor = msgget(LINUX_SERVER_KEY, 0666);
    if (mq_descriptor == -1)
    {
        throw std::runtime_error("Cannot get a message queue");
    }
}

logger const *server_logger::log(
    const std::string &text,
    logger::severity severity) const noexcept
{
    std::mutex mutex;

    for (auto &curr_stream: _data)
    {
        if (curr_stream.second.find(severity) != curr_stream.second.end())
        {
            std::lock_guard<std::mutex> lock(mutex);
            msg_q message{};
            message.m_type = MAIN_PRIOR;
            message.pid = getpid();
            message.severity = static_cast<int>(severity);
            message.packet_cnt = 1 + (text.size() / MAX_TEXT_SIZE);
            for (int i = 1, position = 0; i <= message.packet_cnt; ++i, position += MAX_TEXT_SIZE)
            {
                strcpy(message.m_text, text.substr(position, MAX_TEXT_SIZE).data());
                message.packet_id = i;
                if (curr_stream.first[0])
                {
                    strcpy(message.file_path, curr_stream.first.data());
                }
                else
                {
                    strcpy(message.file_path, "");
                }
                msgsnd(mq_descriptor, &message, sizeof(msg_q) - sizeof(long), 0);
            }
        }
    }
    return this;
}
