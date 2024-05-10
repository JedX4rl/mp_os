
#include "../include/client_logger_builder.h"
#include "client_logger.h"
#include <filesystem>

int main()
{
    auto* new_logger = new client_logger_builder("%m [%t]");
    logger* log;
    try
    {
        log = new_logger
                ->add_console_stream(logger::severity::information)->transform_with_configuration("/Users/nikitatretakov/mp_os/logger/client_logger/tests/testing.json", "logger")->build();
    }
    catch (std::exception const &err)
    {
        delete new_logger;
        std::cout << err.what() << std::endl;
        return 0;
    }
    log->log("HAHAHHA", logger::severity::information);
    log->log("SHOOTING))))))", logger::severity::warning);

    delete new_logger;
    delete log;
    return 0;
}
