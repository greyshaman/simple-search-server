#include <iostream>
#include <string>
#include <QCoreApplication>
#include <exception>

#include "converterjson.h"
#include "inverted-index.h"
#include "search-server.h"

int main()
{
  std::string config_filename{"../../../test/testSearchServer/debug/config.json"};
  std::string requests_filename{"../../../test/testSearchServer/debug/requests.json"};
  std::string answers_filename{"./answers.json"};

  try {
    search_server::ConverterJSON converter(config_filename, requests_filename, answers_filename);
    search_server::inverted_index::InvertedIndex idx;
    idx.UpdateDocumentBase(converter.GetTextDocuments());
    search_server::SearchServer srv(idx);
    srv.search(converter.GetRequests());
  } catch (const std::exception& ex) {
    std::cerr << "Exception occured: " << ex.what() << std::endl;
  }

  return 0;
}
