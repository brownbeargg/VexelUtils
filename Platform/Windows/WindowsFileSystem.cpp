#include "Files/FileSystem.hpp"

#include <windows.h>

namespace Vex
{
    std::filesystem::path FileSystem::GetExecutablePath()
    {
        wchar_t buffer[MAX_PATH];
        DWORD size = GetModuleFileNameW(nullptr, buffer, MAX_PATH);

        return std::filesystem::path(buffer);
    }

    std::filesystem::path FileSystem::GetExecutableDir()
    {
        return GetExecutablePath().parent_path();
    }
} // namespace Vex
