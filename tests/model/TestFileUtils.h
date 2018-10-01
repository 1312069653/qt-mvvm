#include <gtest/gtest.h>
#include "fileutils.h"
#include <string>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <stdexcept>

namespace
{
const std::string projectDir = "test_ProjectUtils";
}

class TestFileUtils : public ::testing::Test
{
public:
    ~TestFileUtils();

    //! Helper function to create test file in a given directory (directory should exist).
    void createTestFile(const std::string& dirname, const std::string& fileName)
    {
        std::string filename = dirname.empty() ? fileName : dirname + "/" + fileName;

        QFile file(QString::fromStdString(filename));
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            throw std::runtime_error("TestFileUtils::createTestFile() -> Error. "
                                     "Can't create file");

        QTextStream out(&file);
        out << "Test file " << 42 << "\n";
        file.close();
    }

};

TestFileUtils::~TestFileUtils() = default;

TEST_F(TestFileUtils, initialState)
{
    QDir dir(QString::fromStdString(projectDir));
    if (dir.exists()) {
        EXPECT_TRUE(Utils::removeRecursively(projectDir) == true);
        EXPECT_TRUE(dir.exists() == false);
    }

    Utils::create_subdir(".", projectDir);
    EXPECT_TRUE(Utils::exists(projectDir));

    createTestFile(projectDir, "a.txt");
    EXPECT_TRUE(Utils::exists(projectDir + "/a.txt"));
}