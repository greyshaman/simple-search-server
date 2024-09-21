#include <QtTest/QTest>
#include <string>
#include <iostream>
#include <sstream>
#include <exception>
#include <fstream>
#include <filesystem>
#include <cfloat>
#include <nlohmann/json.hpp>

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

using json = nlohmann::json;
using namespace search_server;

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
  void testNoMaxResponsesSection();
  void testGetTextDocuments();
  void testResponsesLimit();

  void testGetRequestsWhenMissingFile();
  void testGetRequestsWithEmptyFile();
  void testGetRequestsPositive();

  void testPutAnswers();
};

TestConverterJSON::TestConverterJSON() {}

TestConverterJSON::~TestConverterJSON() {}

void TestConverterJSON::testShouldThrowExceptionWhenConfigMissed() {
  QVERIFY_EXCEPTION_THROWN(
    ConverterJSON("unexisted_config.json", TEST_REQUESTS_FILENAME, TEST_ANSWERS_FILENAME),
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

void TestConverterJSON::testNoMaxResponsesSection() {
  QVERIFY_EXCEPTION_THROWN(
      ConverterJSON(TESTS_SOURCE_DIR"/confs/no_max_responses_config.json", TEST_REQUESTS_FILENAME, TEST_ANSWERS_FILENAME),
      NoMaxResponsesException
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
                            TEST_ANSWERS_FILENAME);
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
                            TEST_ANSWERS_FILENAME);
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
                          TEST_ANSWERS_FILENAME);
  std::vector<std::string> requests = converter.GetRequests();

  QCOMPARE(requests.size(), 3);
}

void TestConverterJSON::testPutAnswers()
{
  ConverterJSON converter(TESTS_SOURCE_DIR "/confs/good_config.json",
                          TESTS_SOURCE_DIR "/requests/requests.json",
                          TEST_ANSWERS_FILENAME);

  std::vector<std::vector<std::pair<int, float>>> mockAnswers({
      {std::vector<std::pair<int, float>>()},
      {{std::pair(0, 0.99)}},
      {{std::pair(0, 0.99)}, {std::pair(1, 0.96)}},
  });

  try {
    using namespace nlohmann::literals;
    converter.PutAnswers(mockAnswers);

    QVERIFY2(std::filesystem::is_regular_file(TEST_ANSWERS_FILENAME), "The answers file not found");

	std::ifstream file(TEST_ANSWERS_FILENAME);
	QVERIFY2(file.is_open(), "The answers file cannot open");

	json jData = json::parse(file);
	QVERIFY2(jData.contains("/answers"_json_pointer),
			 "The answers file should have 'answers' root node");

	QVERIFY2(jData.contains("/answers/request001"_json_pointer),
			 "The answers file should have '/answers/request001' node");
	QVERIFY2(jData.contains("/answers/request001/result"_json_pointer),
			 "The answers file should have '/answers/request001/result' node");
	QVERIFY2(jData.at("/answers/request001/result"_json_pointer) == "false",
			 "The '/answers/request001/result' node should have 'false' value at answers file");

	QVERIFY2(jData.contains("/answers/request002"_json_pointer),
			 "The answers file should have '/answers/request002' node");
	QVERIFY2(jData.contains("/answers/request002/result"_json_pointer),
			 "The answers file should have '/answers/request002/result' node");
	QVERIFY2(jData.at("/answers/request002/result"_json_pointer) == "true",
			 "The '/answers/request002/result' node should have 'true' value at answers file");
	QVERIFY2(jData.contains("/answers/request002/docid"_json_pointer),
			 "The answers file should have '/answers/request002/docid' node");
	QVERIFY2(jData.at("/answers/request002/docid"_json_pointer) == 0,
			 "The '/answers/request002/docid' node should have 0 value at answers file");
	QVERIFY2(jData.contains("/answers/request002/rank"_json_pointer),
			 "The answers file should have '/answers/request002/rank' node");
	{
	  const float rank = jData.at("/answers/request002/rank"_json_pointer);
	  QVERIFY2(std::abs(rank - 0.99) < FLT_EPSILON,
			   "The '/answers/request002/rank' node should have 0.99 value at answers file");
	}

	QVERIFY2(jData.contains("/answers/request003"_json_pointer),
			 "The answers file should have '/answers/request003' node");
	QVERIFY2(jData.contains("/answers/request003/result"_json_pointer),
			 "The answers file should have '/answers/request003/result' node");
	QVERIFY2(jData.at("/answers/request003/result"_json_pointer) == "true",
			 "The '/answers/request003/result' node should have 'true' value at answers file");
	QVERIFY2(jData.contains("/answers/request003/relevance"_json_pointer),
			 "The answers file should have '/answers/request003/relevance' node");
	{
	  const std::vector<json> relevanceList = jData.at("/answers/request003/relevance"_json_pointer);
	  QCOMPARE(relevanceList.size(), 2);
	  auto relevanceListItr = relevanceList.begin();
	  {
		const auto node = *relevanceListItr;
		QVERIFY2(
			node.contains("docid"),
			"The first relevance item in '/answers/request003/relevance' should have docid node");
		QCOMPARE((int) node.at("docid"), 0);
		QVERIFY2(
			node.contains("rank"),
			"The first relevance item in '/answers/request003/relevance' should have rank node");
		const float rank = node.at("rank");
		QVERIFY2(
			std::abs(rank - 0.99) < FLT_EPSILON,
			"The rank in first item of '/answers/request003/relevance' should have 0.99 value");
	  }
	  ++relevanceListItr;
	  {
		const auto node = *relevanceListItr;
		QVERIFY2(
			node.contains("docid"),
			"The second relevance item in '/answers/request003/relevance' should have docid node");
		QCOMPARE((int) node.at("docid"), 1);
		QVERIFY2(
			node.contains("rank"),
			"The second relevance item in '/answers/request003/relevance' should have rank node");
		const float rank = node.at("rank");
		QVERIFY2(
			std::abs(rank - 0.96) < FLT_EPSILON,
			"The rank in second item of '/answers/request003/relevance' should have 0.96 value");
	  }
	}

    std::filesystem::remove(TEST_ANSWERS_FILENAME);
  } catch (const std::exception& ex) {
    if (std::filesystem::is_regular_file(TEST_ANSWERS_FILENAME)) {
      std::filesystem::remove(TEST_ANSWERS_FILENAME);
    }
    std::stringstream ss;
    ss << "Test is unterrupted by eception: " << ex.what();
    QFAIL(ss.str().c_str());
  }

}

QTEST_APPLESS_MAIN(TestConverterJSON)

#include "test_converterjson.moc"
