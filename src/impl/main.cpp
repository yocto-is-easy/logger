#include <logger.hpp>

#include "file_logger.hpp"

const std::string DEFAULT_LOG_FILEPATH = "/var/mocto/log";

INIT_MIDF_SERVER(logger)

MIDF_IMPL_FUNC(bool, logger, log, std::string/*who*/, std::string/*message*/)
    (std::string who, std::string msg) {
    static file_logger fl(DEFAULT_LOG_FILEPATH);
    fl.log(who + ": " + msg);

    return true;
}

int main() {
    START_MIDF_SERVER_WITHOUT_OBSERVER(logger);
}
