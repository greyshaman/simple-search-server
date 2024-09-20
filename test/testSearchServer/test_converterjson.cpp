#include <QtTest/QTest>
#include <string>
#include <iostream>
#include <sstream>
#include <exception>

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
  void testGetTextDocuments();
  void testResponsesLimit();

  void testGetRequestsWhenMissingFile();
  void testGetRequestsWithEmptyFile();
  void testGetRequestsPositive();

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
    ConverterJSON(TESTS_SOURCE_DIR"/confs/wo_required_config.json", TEST_REQUESTS_FILENAME, TEST_ANSWERS_FILENAME),
    ConfigFileIsEmptyException
  );
}

void TestConverterJSON::testIncorrectVersion() {
  QVERIFY_EXCEPTION_THROWN(
    ConverterJSON(TESTS_SOURCE_DIR"/confs/incorrect_version_config.json", TEST_REQUESTS_FILENAME, TEST_ANSWERS_FILENAME),
    IncorrectVersionException
  );
}

void TestConverterJSON::testFileSection() {
  QVERIFY_EXCEPTION_THROWN(
    ConverterJSON(TESTS_SOURCE_DIR"/confs/wo_files_section_config.json", TEST_REQUESTS_FILENAME, TEST_ANSWERS_FILENAME),
    FilesSectionMissingException
  );
}

void TestConverterJSON::testGetTextDocuments() {
  std::stringstream errorLog;
  std::streambuf* originalBuffer = std::cerr.rdbuf();
  std::cerr.rdbuf(errorLog.rdbuf());

  try {
    ConverterJSON converter(
      TESTS_SOURCE_DIR"/confs/good_config.json", TEST_REQUESTS_FILENAME, TEST_ANSWERS_FILENAME
    );
    std::vector<std::string> resourceFilenames = converter.GetTextDocuments();
    std::string errorMessage = errorLog.str();
    QCOMPARE(errorMessage, "File ../resources/missed_file.txt not found\n");
    std::cerr.rdbuf(originalBuffer);
    QCOMPARE(resourceFilenames.size(), 1);

  } catch (const std::exception& ex) {
    std::cerr.rdbuf(originalBuffer);
    std::stringstream ss;
    ss << "Test is interrupted by exception: " << ex.what();
    QFAIL(ss.str().c_str());
  }
}

void TestConverterJSON::testResponsesLimit() {
  QVERIFY_EXCEPTION_THROWN(
    ConverterJSON(TESTS_SOURCE_DIR"/confs/no_max_responses_config.json", TEST_REQUESTS_FILENAME, TEST_ANSWERS_FILENAME),
    NoMaxResponsesException
  );
}

void TestConverterJSON::testGetRequestsWhenMissingFile()
{
  std::stringstream errorLog;
  std::streambuf* originalBuffer = std::cerr.rdbuf();
  std::cerr.rdbuf(errorLog.rdbuf());

  try {
    ConverterJSON converter(TESTS_SOURCE_DIR "/confs/good_config.json",
                            TESTS_SOURCE_DIR "/requests/missing.json",
                            TEST_REQUESTS_FILENAME);
    std::vector<std::string> requests = converter.GetRequests();

    std::cerr.rdbuf(originalBuffer);

	QCOMPARE(errorLog.str(), "The file " TESTS_SOURCE_DIR "/requests/missing.json is missing\n");
	QCOMPARE(requests.size(), 0);
  } catch (std::exception& ex) {
    std::cerr.rdbuf(originalBuffer);
    std::stringstream ss;
    ss << "Test is interrupted by exception: " << ex.what();
    QFAIL(ss.str().c_str());
  }
}

void TestConverterJSON::testGetRequestsWithEmptyFile()
{
  std::stringstream errorLog;
  std::streambuf* originalBuffer = std::cerr.rdbuf();
  std::cerr.rdbuf(errorLog.rdbuf());

  try {
    ConverterJSON converter(TESTS_SOURCE_DIR "/confs/good_config.json",
                            TESTS_SOURCE_DIR "/requests/no_requests_section.json",
                            TEST_REQUESTS_FILENAME);
    std::vector<std::string> requests = converter.GetRequests();

    std::cerr.rdbuf(originalBuffer);

	QCOMPARE(errorLog.str(), "");
	QCOMPARE(requests.size(), 0);
  } catch (const std::exception& ex) {
    std::cerr.rdbuf(originalBuffer);
    std::stringstream ss;
    ss << "Test is interrupted by exception: " << ex.what();
    QFAIL(ss.str().c_str());
  }
}


void TestConverterJSON::testGetRequestsPositive() {
  ConverterJSON converter(TESTS_SOURCE_DIR "/confs/good_config.json",
                          TESTS_SOURCE_DIR "/requests/requests.json",
                          TEST_REQUESTS_FILENAME);
  std::vector<std::string> requests = converter.GetRequests();

  QCOMPARE(requests.size(), 3);
}

QTEST_APPLESS_MAIN(TestConverterJSON)

#include "test_converterjson.moc"
