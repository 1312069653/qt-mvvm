#include <gtest/gtest.h>
#include "jsonvariant.h"
#include "jsonkeys.h"
#include "testconfig.h"
#include "fileutils.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

//! Test convertion of QVariant from/to QJsonObject.


class TestJsonVariant : public ::testing::Test
{
public:
    ~TestJsonVariant();

    static std::string projectDir() {
        return Testing::TestOutputDir() + "/" + "test_JsonVariant";
    }

    static QJsonArray m_array;

    static void SetUpTestCase() {
        Utils::create_dir(projectDir());
    }

    static void TearDownTestCase() {
        QFile saveFile(QString::fromStdString(projectDir()) + "/variants.json");

        if (!saveFile.open(QIODevice::WriteOnly))
            throw std::runtime_error("TestJsonBasics::save_object() -> Can't save file");

        // saving our variants to file
        QJsonDocument saveDoc(m_array);
        saveFile.write(saveDoc.toJson());
     }
};

QJsonArray TestJsonVariant::m_array = QJsonArray();

TestJsonVariant::~TestJsonVariant() = default;

//! Invalid QVariant convertion.

TEST_F(TestJsonVariant, invalidVariant)
{
    QVariant variant;

    // from variant to json object
    auto object = json::get_json(variant);
    m_array.append(object);

    EXPECT_EQ(object.size(), 2);
    QStringList expected = QStringList() << json::variantTypeKey << json::variantValueKey;
    EXPECT_EQ(object.keys(), expected);
    EXPECT_EQ(object[json::variantTypeKey], json::invalid_type_name);
    EXPECT_TRUE(object[json::variantValueKey].isNull());

    // from json object to variant
    QJsonObject object2{
        {json::variantTypeKey, json::invalid_type_name},
        {json::variantValueKey, QJsonValue()}
    };
    QVariant variant2 = json::get_variant(object2);
    EXPECT_FALSE(variant2.isValid());

    // final comparison
    EXPECT_EQ(variant, variant2);
}

//! Int QVariant convertion.

TEST_F(TestJsonVariant, intVariant)
{
    const int value(42);
    QVariant variant(value);

    // from variant to json object
    auto object = json::get_json(variant);
    m_array.append(object);

    EXPECT_EQ(object.size(), 2);
    QStringList expected = QStringList() << json::variantTypeKey << json::variantValueKey;
    EXPECT_EQ(object.keys(), expected);
    EXPECT_EQ(object[json::variantTypeKey], json::int_type_name);
    EXPECT_EQ(object[json::variantValueKey].toInt(), value);

    // from json object to variant
    QJsonObject object2{
        {json::variantTypeKey, json::int_type_name},
        {json::variantValueKey, value}
    };
    QVariant variant2 = json::get_variant(object2);
    EXPECT_EQ(variant2.toInt(), value);

    // final comparison
    EXPECT_EQ(variant, variant2);
}

//! std::string QVariant convertion.

TEST_F(TestJsonVariant, stringVariant)
{
    const std::string value("abc");
    QVariant variant = QVariant::fromValue(value);

    // from variant to json object
    auto object = json::get_json(variant);
    m_array.append(object);

    EXPECT_EQ(object.size(), 2);
    QStringList expected = QStringList() << json::variantTypeKey << json::variantValueKey;
    EXPECT_EQ(object.keys(), expected);
    EXPECT_EQ(object[json::variantTypeKey], json::string_type_name);
    QVariant var = object[json::variantValueKey].toVariant();
    // variant obtained from json is always based on QString, nave to convert std::string
    // to QString to make comparison
    EXPECT_EQ(object[json::variantValueKey].toString(), QString::fromStdString(value));

    // from json object to variant
    QJsonObject object2{
        {json::variantTypeKey, json::string_type_name},
        {json::variantValueKey, QString::fromStdString(value)}
    };
    QVariant variant2 = json::get_variant(object2);
    EXPECT_EQ(variant2.value<std::string>(), value);
}