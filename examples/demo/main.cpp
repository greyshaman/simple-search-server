#include <iostream>
#include <exception>

#include "converterjson.h"
#include "inverted-index.h"
#include "search-server.h"

int main()
{

  try {
    search_server::ConverterJSON converter;
    std::cout << "Starting " << converter.GetName() << " version " << converter.GetVersion()
              << std::endl;

    search_server::inverted_index::InvertedIndex idx;
    idx.UpdateDocumentBase(converter.GetTextDocuments());
    std::cout << "Texts are indexed\n";

    std::cout << "Reading requests\n";
    search_server::SearchServer srv(idx);
    auto search_result = srv.search(converter.GetRequests());
    std::cout << "Search results complete and will write into result file\n";
    converter.PutAnswers(search_result);
    std::cout << "Answers wrote into file\n";
    std::cout << "Program complete. Thank you.\nBye\n";
  } catch (const std::exception& ex) {
    std::cerr << "Exception occured: " << ex.what() << std::endl;
  }

  return 0;
}
