#include "FileSystem.hpp"

#include "Vexel/Utils/Base.hpp"

namespace Vex
{
    void FileSystem::Mount(RootDirectory root, const std::filesystem::path& path)
    {
        s_Roots[root] = std::filesystem::absolute(path);
    }

    std::filesystem::path FileSystem::Resolve(RootDirectory root, const std::filesystem::path& relativePath)
    {
        std::filesystem::path base = s_Roots.at(root);
        std::filesystem::path fullPath = std::filesystem::weakly_canonical(base / relativePath);

        VEX_RELEASE_ASSERT(IsInside(base, fullPath), "Path is not in filesystem root");

        return fullPath;
    }

    std::string FileSystem::ReadFile(
        RootDirectory root, const std::filesystem::path& relativePath, std::ios::openmode flags)
    {
        std::filesystem::path path = Resolve(root, relativePath);
        std::ifstream file(path, flags);

        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }

    std::filesystem::path FileSystem::GetProjectRoot(const std::filesystem::path& startPath)
    {
        std::filesystem::path currentPath = std::filesystem::absolute(startPath);

        if (!std::filesystem::is_directory(currentPath))
        {
            currentPath = currentPath.parent_path();
        }

        const char* token = "Vexel.project";

        while (currentPath != currentPath.parent_path())
        {
            if (std::filesystem::exists(currentPath / token))
                return currentPath;

            currentPath = currentPath.parent_path();
        }

        return {};
    }

    bool FileSystem::IsInside(const std::filesystem::path& r, const std::filesystem::path& p)
    {
        std::filesystem::path root = std::filesystem::weakly_canonical(r);
        std::filesystem::path path = std::filesystem::weakly_canonical(p);

        std::filesystem::path::iterator rootIt = root.begin();
        std::filesystem::path::iterator pathIt = path.begin();

        for (; rootIt != root.end() && pathIt != path.end(); ++rootIt, ++pathIt)
            if (pathIt == path.end() || *pathIt != *rootIt)
                return false;

        return true;
    }

    std::filesystem::path FileSystem::GetExecutableDir()
    {
        return GetExecutablePath().parent_path();
    }

    std::filesystem::path FileSystem::GetBuildDir()
    {
        std::filesystem::path currentPath = GetExecutableDir();

        const char* token = "vexel.build";

        while (currentPath != currentPath.parent_path())
        {
            if (std::filesystem::exists(currentPath / token))
                return currentPath;

            currentPath = currentPath.parent_path();
        }

        return {};
    }
} // namespace Vex
