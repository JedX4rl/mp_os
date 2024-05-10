#include <server_logger.h>
#include <server_logger_builder.h>

int main()
{
    auto* new_logger = new server_logger_builder();
    logger* log;
    try
    {
        log = new_logger->add_console_stream(logger::severity::information)->add_file_stream("m", logger::severity::information)->build();
//                ->add_console_stream(logger::severity::information)->transform_with_configuration("/Users/nikitatretakov/mp_os/logger/client_logger/tests/testing.json", "logger")->build();
    }
    catch (std::exception const &err)
    {
        delete new_logger;
        std::cout << err.what() << std::endl;
        return 0;
    }
    log->log("1234567890/1234567890/1234", logger::severity::information);
    log->log("SHOOTING))))))", logger::severity::warning);

    delete new_logger;
    delete log;
    return 0;
}