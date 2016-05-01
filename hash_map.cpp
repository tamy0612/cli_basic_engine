#include <algorithm>
#include <string>

#include <cctype>

#include "hash_map.hpp"


using namespace cli;

namespace {
  std::hash<std::string> to_hash;
}


bool InsensitiveEqual::operator()(const std::string& lhs, const std::string& rhs) const noexcept
{
  if(lhs.size() != rhs.size()) return false;
  for( auto i = 0; i < lhs.size(); ++i ) {
    if(::tolower(lhs[i]) != ::tolower(rhs[i])) {
      return false;
    }
  }
  return true;
}


auto InsensitiveHash::operator()(std::string str) const noexcept
  -> InsensitiveHash::result_type
{
  std::transform( str.begin(), str.end(), str.begin(), ::tolower );
  return to_hash( str );
}
