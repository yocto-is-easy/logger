#pragma once

#include <string>

#include <midf.hpp>

MIDF_DECL_PORT(logger, 1811)

MIDF_DECL_FUNC(bool, logger, log, std::string/*who*/, std::string/*message*/);
