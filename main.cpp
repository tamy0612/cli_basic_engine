#include <iostream>
#include <exception>

#include "engine.hpp"


using cli::Engine;

int main(int argc, char const* argv[])
{
  Engine engine;
  engine.initialize(argc, argv);
  try {
    engine.main_loop( std::cin, std::cout );
  } catch(const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
