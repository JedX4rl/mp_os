
#include "../include/client_logger_builder.h"
#include "client_logger.h"
#include <filesystem>

int main()
{
    auto* new_logger = new client_logger_builder("%f %t %m");
    logger* log = new_logger
            ->add_console_stream(logger::severity::information)->build();
    log->log("HAHAHHA", logger::severity::information);
    log->log("SHOOTING))))))", logger::severity::warning);

    delete new_logger;
    delete log;
}
