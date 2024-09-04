#include "files-section-missing-exception.h"

FilesSectionMissingException::FilesSectionMissingException(const std::string& inFilename)
  : ConfigParserException(inFilename + "required files section") {}
