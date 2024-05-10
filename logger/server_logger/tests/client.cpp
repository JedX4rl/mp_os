//
// Created by Никита Третьяков on 28.03.2024.
//
#include <sys/msg.h>
#include <sys/ipc.h>
#include <iostream>
#include <unistd.h>
#include <logger.h>


#define LINUX_SERVER_KEY 100
#define MAX_TEXT_SIZE 10
#define MAIN_PRIOR 2

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

int main()
{
    std::cout << "DADADAWDA";
    msg_q msg{};
    int mq_d = msgget(LINUX_SERVER_KEY, 0666);
    if (mq_d == -1)
    {
        std::cout << "Cannot create a message queue" << std::endl;
        return 1;
    }

    msg.m_type = MAIN_PRIOR;
    msg.pid = getpid();
    msg.packet_cnt = 2;
    msg.packet_id = 1;
    msg.severity = 2;
    strcpy(msg.file_path, "");
    strcpy(msg.m_text, "Shooting");

    msgsnd(mq_d, &msg, sizeof(msg_q) - sizeof(long), 0);

    msg.packet_id = 2;
    strcpy(msg.m_text, "in school");

    msgsnd(mq_d, &msg, sizeof(msg_q) - sizeof(long), 0);
    return 0;


}