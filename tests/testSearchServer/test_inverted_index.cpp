#include <QtTest/QTest>
#include <vector>
#include <string>
#include <algorithm>

#include "inverted-index.h"

using namespace search_server::inverted_index;
using namespace std;

class TestInvertedIndex : public QObject
{
  Q_OBJECT

  void checkInvertedIndexFunctionality(const vector<string>& docs,
                                       const vector<string>& requests,
                                       const vector<vector<Entry>>& expected);

  void deepSort(vector<vector<Entry>>& source);

public:
  TestInvertedIndex() = default;
  ~TestInvertedIndex();

private slots:
  void testBasic1();
  void testBasic2();
  void testMissingWord();
};

void TestInvertedIndex::checkInvertedIndexFunctionality(
	const vector<string>& docs,
	const vector<string>& requests,
	const vector<vector<Entry>>& expected)
{
  vector<vector<Entry>> result;
  InvertedIndex idx;

  idx.UpdateDocumentBase(docs);

  for (auto& request : requests) {
    auto word_count = idx.GetWordCount(request);
    result.push_back(word_count);
  }

  deepSort(result);
  QCOMPARE(result, expected);
}

void TestInvertedIndex::deepSort(vector<vector<Entry>>& source)
{
  for (auto& entries : source) {
    sort(entries.begin(), entries.end());
  }
}

TestInvertedIndex::~TestInvertedIndex() {}

void TestInvertedIndex::testBasic1()
{
  const vector<string> docs = {"london is the capital of great britain",
                               "big ben is the nickname for the Great bell of the striking clock"};
  const vector<string> requests = {"london", "the"};
  const vector<vector<Entry>> expected = {{{0, 1}}, {{0, 1}, {1, 3}}};

  checkInvertedIndexFunctionality(docs, requests, expected);
}

void TestInvertedIndex::testBasic2()
{
  const vector<string> docs = {"milk milk milk milk water water water",
                               "milk water water",
                               "milk milk milk milk milk water water water water water",
                               "americano cappuccino"};
  const vector<string> requests = {"milk", "water", "cappuccino"};
  const vector<vector<Entry>> expected = {{{0, 4}, {1, 1}, {2, 5}},
                                          {{0, 3}, {1, 2}, {2, 5}},
                                          {{3, 1}}};

  checkInvertedIndexFunctionality(docs, requests, expected);
}

void TestInvertedIndex::testMissingWord()
{
  const vector<string> docs = {"a b c d e f g h i j k l", "statement"};
  const vector<string> requests = {"m", "statement"};
  const vector<vector<Entry>> expected = {{}, {{1, 1}}};

  checkInvertedIndexFunctionality(docs, requests, expected);
}

QTEST_APPLESS_MAIN(TestInvertedIndex)

#include "test_inverted_index.moc"
