//
// Created by Ivars Rusbergs in 2021
//
// Alpha application entry point
#include <exception>
#include <iostream>

#include "alpha.h"

int main(int argc, char *argv[]) {
  try {
    Alpha app(argc, argv);
    app.Run();
  } catch (const std::exception &except) {
    std::cout << "Exception: " << except.what() << std::endl;
    return 1;
  } catch (...) {
    std::cout << "Unknown error" << std::endl;
    return 1;
  }

  return 0;
}