#pragma once

#include <string>
#include <filesystem>

#include "core.h"

// resolves relative to this source file's directory (build-machine only;
// kept for the legacy platformer example)
std::string NYL_API getFullPath(const std::string& relativePath);

// directory containing the running executable (deployment-safe)
std::string NYL_API getExecutableDir();