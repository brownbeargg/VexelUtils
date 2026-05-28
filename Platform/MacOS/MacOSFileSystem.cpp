/*
 * WARNING: THIS FILE HAS NOT BEEN TESTED PLEASE USE WITH CAUTION
 * ISSUE: THERE IS NO MacOS COMPUTER TO TEST ON.
 * FIX: IF YOU TEST THIS AND IT WORKS PLEASE ISSUE A PR
 */

#include "Files/FileSystem.hpp"

#include <mach-o/dyld.h>
#include <limits.h>

namespace Vex
{
    std::filesystem::path FileSystem::GetExecutablePath()
    {
        char buffer[PATH_MAX];
        uint32_t size = sizeof(buffer);

        if (_NSGetExecutablePath(buffer, &size) != 0)
            return {};

        return std::filesystem::canonical(buffer);
    }
} // namespace Vex
