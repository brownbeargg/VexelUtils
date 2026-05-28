#pragma once

#include "Vexel/Utils/STL.hpp"

namespace Vex
{
    enum class RootDirectory
    {
        Assets,
        Config,
        Save,
        Cache,
        Engine,
        Root,
        Build,
        Binary,
    };

    class FileSystem
    {
      public:
        static void Mount(RootDirectory root, const std::filesystem::path& path);
        static bool IsMounted(RootDirectory root) { return s_Roots.count(root); }

        static std::filesystem::path Resolve(RootDirectory root, const std::filesystem::path& relativePath);
        static std::string ReadText(RootDirectory root, const std::filesystem::path& relativePath);

        static const std::filesystem::path& GetRootDirectoryPath(RootDirectory root) { return s_Roots[root]; }
        static std::filesystem::path GetProjectRoot(const std::filesystem::path& startPath);

        static std::filesystem::path GetExecutablePath();
        static std::filesystem::path GetExecutableDir();

      private:
        static bool IsInside(const std::filesystem::path& root, const std::filesystem::path& path);

      private:
        inline static std::unordered_map<RootDirectory, std::filesystem::path> s_Roots = {
            {RootDirectory::Root, std::filesystem::current_path().root_directory()}};
    };
} // namespace Vex
