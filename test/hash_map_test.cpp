#include <string>
#include <boost/test/unit_test.hpp>

#include "../hash_map.hpp"


using namespace cli;


BOOST_AUTO_TEST_SUITE( hash_map_test )

  BOOST_AUTO_TEST_CASE( test_insensitive_equal )
  {
    InsensitiveEqual comp;
    std::string str_1 = "test";
    std::string str_2 = "TEST";
    BOOST_CHECK( str_1 != str_2 );
    BOOST_CHECK( comp(str_1,str_2) );
  }

  BOOST_AUTO_TEST_CASE( test_insensitive_hash )
  {
    {
      std::hash<std::string> hash;
      BOOST_CHECK( hash("test") != hash("TEST") );
    }
    {
      InsensitiveHash hash;
      BOOST_CHECK_EQUAL( hash("test"), hash("TEST") );
    }
  }

BOOST_AUTO_TEST_SUITE_END()
