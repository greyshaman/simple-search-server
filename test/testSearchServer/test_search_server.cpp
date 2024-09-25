#include <QtTest/QTest>
#include <vector>
#include <string>

#include "converterjson.h"
#include "search-server.h"

#include "config-test.h"
#include "test_search_server.h"

using namespace std;

class TestSearchServer : public QObject
{
  Q_OBJECT

public:
  TestSearchServer() = default;
  ~TestSearchServer();

private slots:
  void testSimple();
  void testTop5();
};

TestSearchServer::~TestSearchServer() {}

void TestSearchServer::testSimple()
{
  const vector<string> docs = {"milk milk milk milk water water water",
                               "milk water water",
                               "milk milk milk milk milk water water water water water",
                               "americano cappuccino"};
  const vector<string> request = {"milk water", "sugar"};
  const std::vector<vector<search_server::RelativeRelevance>> expected
      = {{{2, 1}, {0, 0.7}, {1, 0.3}}, {}};

  search_server::inverted_index::InvertedIndex idx;
  idx.UpdateDocumentBase(docs);

  search_server::SearchServer srv(idx);

  vector<vector<search_server::RelativeRelevance>> result = srv.search(request);
  QCOMPARE(result, expected);
}

void TestSearchServer::testTop5()
{
  const vector<string> docs = {
      "london is the capital of great britain", // 0
      "paris is the capital of france", // 1
      "berlin is the capital of germany", // 2
      "rome is the capital of italy", // 3
      "madrid is the capital of spain", // 4
      "lisboa is the capital of portugal", // 5
      "bern is the capital of switzerland", // 6
      "moscow is the capital of russia", // 7
      "kiev is the capital of ukraine", // 8
      "minsk is the capital of belarus", // 9
      "astana is the capital of kazakhstan", // 10
      "beijing is the capital of china", // 11
      "tokyo is the capital of japan", // 12
      "bangkok is the capital of thailand", // 13
      "welcome to moscow the capital of russia the third rome", // 14
      "amsterdam is the capital of netherlands", // 15
      "helsinki is the capital of finland", // 16
      "oslo is the capital of norway", // 17
      "stockholm is the capital of sweden", // 18
      "riga is the capital of latvia", // 19
      "tallinn is the capital of estonia", // 20
      "warsaw is the capital of poland", // 21
  };

  const vector<string> request = {"moscow is the capital of russia"};

  const std::vector<vector<search_server::RelativeRelevance>> expected = {
      {
          {7, 1},
          {14, 1},
          {0, 0.666666687},
          {1, 0.666666687},
          {2, 0.666666687}
      }
  };

  search_server::ConverterJSON converter(
      TESTS_SOURCE_DIR"/confs/good_config.json", TEST_REQUESTS_FILENAME, TEST_ANSWERS_FILENAME
      );

  search_server::inverted_index::InvertedIndex idx;
  idx.UpdateDocumentBase(docs);

  search_server::SearchServer srv(idx);

  vector<vector<search_server::RelativeRelevance>> result = srv.search(request, converter.GetResponsesLimit());
  QCOMPARE(result, expected);
}

QTEST_APPLESS_MAIN(TestSearchServer)

#include "test_search_server.moc"
