#include <logger.hpp>

#include "file_logger.hpp"

INIT_MIDF_SERVER(logger)

MIDF_IMPL_FUNC(bool, logger, log, std::string/*who*/, std::string/*message*/)
    (std::string who, std::string msg) {
    static file_logger fl;
    fl.log(who + ": " + msg);

    return true;
}

int main() {
    START_MIDF_SERVER_WITHOUT_OBSERVER(logger);
}
