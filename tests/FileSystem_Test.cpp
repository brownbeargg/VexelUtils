#include <gtest/gtest.h>

#include <Vexel/Base.hpp>
#include <Vexel/Files.hpp>

TEST(FileSystem, Reads)
{
    std::filesystem::path exeDir = Vex::FileSystem::GetExecutableDir();
    std::filesystem::path projectRoot = Vex::FileSystem::GetProjectRoot(exeDir);

    Vex::FileSystem::Mount(Vex::RootDirectory::Assets, projectRoot / "Assets");
    std::string rPath = Vex::FileSystem::GetRootDirectoryPath(Vex::RootDirectory::Assets).string();
    std::string text = Vex::FileSystem::ReadText(Vex::RootDirectory::Assets, "Reads.txt");
    EXPECT_EQ(text == "HELLO WORLD\n", true);
}
