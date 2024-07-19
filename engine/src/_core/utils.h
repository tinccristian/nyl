#pragma once

#include <string>
#include <filesystem>

#include "core.h"

#define BIT(BITPOS)     (1 << (BITPOS))

std::string NYL_API getFullPath(const std::string& relativePath);