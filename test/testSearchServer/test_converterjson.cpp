#include <QtTest/QTest>
#include <string>
#include <fstream>

#include "converterjson.h"
#include "config-file-missing-exception.h"

const std::string TEST_CONFIG_FILENAME = "test_config.json";
const std::string TEST_REQUESTS_FILENAME = "test_requests.json";
const std::string TEST_ANSWERS_FILENAME = "test_answers.json";

class TestConverterJSON : public QObject {
  Q_OBJECT

public:
  TestConverterJSON();
  ~TestConverterJSON();

private slots:
  // Тестировать появление исключения если нет конфигурационного файла
  void testShouldThrowExceptionWhenConfigMissed();
  // Тестировать отсутствие секции config в конфигурационном файле
  void testParsingConfigSectionInConfigurationFile();
  // TODO testing loading files content as strings list
  // void testGetTextDocuments();
  // TODO testing get responses limit from config
  // void testResponsesLimit();
  // TODO testing get requests from json file
  // void testGetRequests();
  // TODO testing put answer
  // void testPutAnswers();
};

TestConverterJSON::TestConverterJSON() {}

TestConverterJSON::~TestConverterJSON() {}

void TestConverterJSON::testShouldThrowExceptionWhenConfigMissed() {
  QVERIFY_EXCEPTION_THROWN(
    ConverterJSON("abscent_config.json", TEST_REQUESTS_FILENAME, TEST_ANSWERS_FILENAME),
    ConfigFileMissingException
  );
}

void TestConverterJSON::testParsingConfigSectionInConfigurationFile() {
  // TODO Implement testcase
}

QTEST_APPLESS_MAIN(TestConverterJSON)

#include "test_converterjson.moc"
