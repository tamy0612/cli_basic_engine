#include <boost/test/unit_test.hpp>
#include <iostream>

#include "../command.hpp"


using namespace cli;


BOOST_AUTO_TEST_SUITE( command_test )

  BOOST_AUTO_TEST_CASE(test_empty_command)
  {
    Command command("");
    BOOST_CHECK_EQUAL( command.name(), "" );
    BOOST_CHECK_EQUAL( command.num_arguments(), 0 );
  }

  BOOST_AUTO_TEST_CASE(test_unary_command)
  {
    Command command;
    command.parse("test");
    BOOST_CHECK_EQUAL( command.name(), "test" );
    BOOST_CHECK_EQUAL( command.num_arguments(), 0 );
  }

  BOOST_AUTO_TEST_CASE(test_command_with_arguments)
  {
    Command command;
    command.parse("this is a test");
    BOOST_CHECK_EQUAL( command.name(), "this" );
    BOOST_CHECK_EQUAL( command.num_arguments(), 3 );
    BOOST_CHECK_EQUAL( command.argument(0), "is" );
    BOOST_CHECK_EQUAL( command.argument(1), "a" );
    BOOST_CHECK_EQUAL( command.argument(2), "test" );
  }

  BOOST_AUTO_TEST_CASE(test_command_overwrite)
  {
    Command command("this is a test");
    command.parse("this is a    over-writing test");
    BOOST_CHECK_EQUAL( command.name(), "this" );
    BOOST_CHECK_EQUAL( command.num_arguments(), 4 );
    BOOST_CHECK_EQUAL( command.argument(0), "is" );
    BOOST_CHECK_EQUAL( command.argument(1), "a" );
    BOOST_CHECK_EQUAL( command.argument(2), "over-writing" );
    BOOST_CHECK_EQUAL( command.argument(3), "test" );
  }

  BOOST_AUTO_TEST_CASE(test_response)
  {
    Command command("This is a test command");
    command.response_stream() << "Complete!" << ' ' << 1 << 234;
    BOOST_CHECK_EQUAL( command.response(), "Complete! 1234" );
    command.parse("This is another command");
    BOOST_CHECK_EQUAL( command.response(), "" );
  }

  BOOST_AUTO_TEST_CASE(test_clear)
  {
    Command command("This is a test command");
    command.response_stream() << "This is a test response";
    command.clear();
    BOOST_CHECK_EQUAL( command.name(), "This" );
    BOOST_CHECK_EQUAL( command.num_arguments(), 4 );
    BOOST_CHECK_EQUAL( command.response(), "" );
  }

BOOST_AUTO_TEST_SUITE_END()
