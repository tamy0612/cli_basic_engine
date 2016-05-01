#define BOOST_TEST_NO_LIB

#if !defined( NDEBUG )

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
//#include <boost/test/unit_test_log.hpp>

#include <iostream>

#endif

#define BOOST_TEST_MODULE boost_unit_test
#include <boost/test/included/unit_test.hpp>
