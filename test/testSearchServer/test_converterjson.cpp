#include <QtTest/QTest>
#include <string>

#include "converterjson.h"

const std::string TEST_CONFIG_FILENAME = "test_config.json";
const std::string TEST_REQUESTS_FILENAME = "test_requests.json";
const std::string TEST_ANSWERS_FILENAME = "test_answers.json";

class TestConverterJSON : public QObject {
  Q_OBJECT

public:
  TestConverterJSON();
  ~TestConverterJSON();

private slots:
  // TODO remove this test case after other test cases will work
  void test1();
  // TODO протестировать появление исключения если нет конфигурационного файла
  void testShouldThrowExceptionWhenConfigMissed();
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

void TestConverterJSON::test1() {
  const int value = 1;
  QVERIFY2(value == 1, "Ожидается наличие целого числа 1");
}

void TestConverterJSON::testShouldThrowExceptionWhenConfigMissed() {
  QVERIFY_EXCEPTION_THROWN(
    ConverterJSON converterJSON(TEST_CONFIG_FILENAME, TEST_REQUESTS_FILENAME, TEST_ANSWERS_FILENAME),
    std::runtime_error
  );
}

QTEST_APPLESS_MAIN(TestConverterJSON)

#include "test_converterjson.moc"
