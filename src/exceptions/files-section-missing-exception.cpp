#include "exceptions/files-section-missing-exception.h"

FilesSectionMissingException::FilesSectionMissingException(const std::string& in_filename)
  : ConfigParserException(in_filename + "required files section") {}
