#include <mutex>
#include <memory>

#include <supervisor.hpp>
#include <logger.hpp>

#include "file_logger.hpp"

using namespace std;

const std::string DEFAULT_ALOG_FILEPATH = "/var/mocto/alog";

INIT_MIDF_SERVER(logger)

unique_ptr<file_logger> fl_async;
MIDF_IMPL_FUNC(bool, logger, alog, std::string/*who*/, std::string/*message*/)
    (std::string who, std::string msg) {
    std::string data = who + ": " + msg;
    fl_async->log_async(data);

    return true;
}

MIDF_IMPL_FUNC(bool, logger, set_max_file_size, uint64_t) (uint64_t size) {
    fl_async->set_max_size(size);
    return true;
}

int main() {
    fl_async = std::make_unique<file_logger>(DEFAULT_ALOG_FILEPATH);

    START_MIDF_SERVER(logger);
}
