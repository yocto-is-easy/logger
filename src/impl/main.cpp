#include <logger.hpp>

#include "file_logger.hpp"

const std::string DEFAULT_LOG_FILEPATH = "/var/mocto/log";
const std::string DEFAULT_ALOG_FILEPATH = "/var/mocto/alog";

INIT_MIDF_SERVER(logger)

MIDF_IMPL_FUNC(bool, logger, log, std::string/*who*/, std::string/*message*/)
    (std::string who, std::string msg) {
    static file_logger fl(DEFAULT_LOG_FILEPATH);
    std::string data = who + ": " + msg;
    fl.log(data);

    return true;
}

MIDF_IMPL_FUNC(bool, logger, alog, std::string/*who*/, std::string/*message*/)
    (std::string who, std::string msg) {
    static file_logger fl(DEFAULT_ALOG_FILEPATH);

    std::string data = who + ": " + msg;
    fl.log_async(data);

    return true;
}

int main() {
    START_MIDF_SERVER_WITHOUT_OBSERVER(logger);
}
