#pragma once

#include <string>

#include <midf.hpp>

MIDF_DECL_PORT(logger, 1811)

// async logger
MIDF_DECL_FUNC(bool, logger, alog, std::string/*who*/, std::string/*message*/);

MIDF_DECL_FUNC(bool, logger, set_max_file_size, uint64_t);
