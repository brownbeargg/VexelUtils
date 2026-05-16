/*
 * WARNING: THIS FILE HAS NOT BEEN TESTED PLEASE USE WITH CAUTION
 * ISSUE: THERE IS NO LINUX COMPUTER TO TEST ON.
 * FIX: IF YOU TEST THIS AND IT WORKS PLEASE ISSUE A PR
 */

#include "Files/FileSystem.hpp"

#include <unistd.h>
#include <limits.h>

namespace Vex
{
    std::filesystem::path FileSystem::GetExecutablePath()
    {
        char buffer[PATH_MAX];

        ssize_t count = readlink("/proc/self/exe", buffer, PATH_MAX);

        if (count == -1)
            return {};

        return std::filesystem::path(std::string(buffer, count));
    }

    std::filesystem::path FileSystem::GetExecutableDir()
    {
        return GetExecutablePath().parent_path();
    }
} // namespace Vex
