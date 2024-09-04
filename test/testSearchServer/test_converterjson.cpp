#include <QtTest/QTest>
#include <string>

#include "config-test.h"
#include "converterjson.h"
#include "config-file-missing-exception.h"
#include "config-file-is-empty-exception.h"
#include "incorrect-version-exception.h"
#include "no-max-responses-exception.h"
#include "files-section-missing-exception.h"

const std::string TEST_CONFIG_FILENAME = "test_config.json";
const std::string TEST_REQUESTS_FILENAME = "test_requests.json";
const std::string TEST_ANSWERS_FILENAME = "test_answers.json";

class TestConverterJSON : public QObject {
  Q_OBJECT

public:
  TestConverterJSON();
  ~TestConverterJSON();

private slots:
  void testShouldThrowExceptionWhenConfigMissed();
  void testIsEmtyConfig();
  void testIncorrectVersion();
  void testFileSection();
  // TODO testing loading files content as strings list
  // void testGetTextDocuments();
  void testResponsesLimit();
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

void TestConverterJSON::testIsEmtyConfig() {
  QVERIFY_EXCEPTION_THROWN(
    ConverterJSON(TESTS_WORK_DIR"/wo_required_config.json", TEST_REQUESTS_FILENAME, TEST_ANSWERS_FILENAME),
    ConfigFileIsEmptyException
  );
}

void TestConverterJSON::testIncorrectVersion() {
  QVERIFY_EXCEPTION_THROWN(
    ConverterJSON(TESTS_WORK_DIR"/incorrect_version_config.json", TEST_REQUESTS_FILENAME, TEST_ANSWERS_FILENAME),
    IncorrectVersionException
  );
}

void TestConverterJSON::testFileSection() {
  QVERIFY_EXCEPTION_THROWN(
    ConverterJSON(TESTS_WORK_DIR"/wo_files_section_config.json", TEST_REQUESTS_FILENAME, TEST_ANSWERS_FILENAME),
    FilesSectionMissingException
  );
}

void TestConverterJSON::testResponsesLimit() {
  QVERIFY_EXCEPTION_THROWN(
    ConverterJSON(TESTS_WORK_DIR"/no_max_responses_config.json", TEST_REQUESTS_FILENAME, TEST_ANSWERS_FILENAME),
    NoMaxResponsesException
  );
}

QTEST_APPLESS_MAIN(TestConverterJSON)

#include "test_converterjson.moc"
