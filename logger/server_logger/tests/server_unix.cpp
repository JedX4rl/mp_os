//
// Created by Никита Третьяков on 26.03.2024.
//

#include <sys/msg.h>
#include <sys/ipc.h>
#include "../../logger/include/logger.h"
#include <iostream>
#include <map>
#include <fstream>
#include <exception>
#include <mutex>
#include <thread>

#define LINUX_SERVER_KEY 100
#define MAX_TEXT_SIZE 10
#define SHUTDOWN_PRIOR 1

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

enum class severity
{
    trace,
    debug,
    information,
    warning,
    error,
    critical
};

std::string decode_severity(int severity)
{
    switch (severity)
    {
        case 0:
            return "TRACE";
        case 1:
            return "DEBUG";
        case 2:
            return "INFORMATION";
        case 3:
            return "WARNING";
        case 4:
            return "ERROR";
        case 5:
            return "CRITICAL";
        default:
            return "INFORMATION";
    }
}

void shut_down(int mq_d)
{
    msgctl(mq_d, IPC_RMID, nullptr);
    std::cout << "server shut down" << std::endl;
}

void terminal_reader(const int mq_d)
{
    msg_q msg{};
    std::string cmd;
    while (std::cin >> cmd)
    {
        if (cmd == "shutdown")
        {
            msg.m_type = SHUTDOWN_PRIOR;
            msgsnd(mq_d, &msg, sizeof(msg_q), 0);
            std::cout << "Server has been closed" << std::endl;
            break;
        }
    }
}

int main()
{

    std::mutex mutex;
    std::map<std::string, std::ofstream> streams;
    msg_q msg = {};
    int mq_d = msgget(LINUX_SERVER_KEY, IPC_CREAT | 0666);
    if (mq_d == -1)
    {
        std::cout << "Cannot create a message queue" << std::endl;
        return 1;
    }
    std::thread cmd_thread(terminal_reader, mq_d);
    while (true)
    {
        long rcv_cnt = msgrcv(mq_d, &msg, sizeof(msg_q) - sizeof(long), 0, MSG_NOERROR);
        if (rcv_cnt == -1)
        {
            std::cout << "An error occurred while receiving the massage" << std::endl;
            break;
        }

        if (msg.m_type == SHUTDOWN_PRIOR)
        {
            break;
        }

        if (msg.file_path[0] && !streams.count(msg.file_path))
        {
            std::lock_guard<std::mutex> lock(mutex);
            std::ofstream fstream;
            try
            {
                fstream = std::ofstream(msg.file_path);
                if (!fstream.is_open())
                {
                    throw std::runtime_error("file cannot be opened");
                }
            }
            catch (const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            streams[msg.file_path] = std::move(fstream);
        }

        if (msg.packet_cnt == 1)
        {
            std::lock_guard<std::mutex> lock(mutex);
            std::ostream* stream_ptr = &std::cout;
            if (msg.file_path[0])
            {
                stream_ptr = &streams[msg.file_path];
            }
            std::ostream& log_stream = *stream_ptr;
            std::string severity = decode_severity(msg.severity);
            log_stream << "[" << severity << "] "  << msg.m_text;
            log_stream.flush();
        }
        else
        {
            std::lock_guard<std::mutex> lock(mutex);
            std::string  tmp_file = "tmp_file_" + std::to_string(msg.pid);
            std::ofstream& stream = streams[tmp_file];

            if (msg.packet_id == 1)
            {
                stream.close();
                stream.open(tmp_file, std::ios::out | std::ios::trunc);
            }
            stream << msg.m_text;
            stream.flush();

            if (msg.packet_id == msg.packet_cnt)
            {
                std::ifstream istream(tmp_file);
                if (istream.is_open())
                {
                    std::ostream* stream_ptr = &std::cout;
                    if (msg.file_path[0])
                    {
                        stream_ptr = &streams[msg.file_path];
                    }
                    std::ostream &_log_stream = *stream_ptr;
                    std::string severity = decode_severity(msg.severity);

                    _log_stream << "[" << severity << "] ";

                    char ch;
                    while (istream >> ch)
                    {
                        _log_stream << ch;
                    }
                    _log_stream.flush();
                    istream.close();
//                    msg.m_type = SHUTDOWN_PRIOR;
//                    msgsnd(mq_d, &msg, sizeof(msg_q) - sizeof(long), 0);
                }
            }
        }
    }

    for (auto &record: streams)
    {
        std::ofstream  &stream = record.second;
        stream.flush();
        stream.close();
    }
    cmd_thread.join();
    return 0;
}