#include <boost/test/unit_test.hpp>

#include "../command.hpp"
#include "../callback.hpp"


namespace {

  using cli::Command;

  void test_command( Command& command )
  {
    command.response_stream() << "called test_command(Command&)";
  }

  class TestEngine {

    public:
    TestEngine( int id ) : _id(id){}

    void test( Command& command )
    {
      command.response_stream() << "called TestEngine#test for instance id: " << _id;
    }

    private:
    int _id;

  };

}


using namespace cli;


BOOST_AUTO_TEST_SUITE( callback_test )

  BOOST_AUTO_TEST_CASE(test_static_callback)
  {
    auto callback_static = CallbackStaticFunction( &test_command );
    Command command;
    callback_static( command );
    BOOST_CHECK_EQUAL( command.response(), "called test_command(Command&)" );
  }

  BOOST_AUTO_TEST_CASE(test_member_callback)
  {
    TestEngine engine(1);
    auto callback_member = CallbackMemberFunction<TestEngine>( &engine, &TestEngine::test );
    Command command;
    callback_member( command );
    BOOST_CHECK_EQUAL( command.response(), "called TestEngine#test for instance id: 1" );
  }

BOOST_AUTO_TEST_SUITE_END()
