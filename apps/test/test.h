//
// Created by Ivars Rusbergs in 2021
//
// Application used as an experimental app for engine testing
#ifndef LSIM_APPS_TEST_H_
#define LSIM_APPS_TEST_H_

#include <lsim/platform/i_application.h>

namespace apps::test {
class Test : public lsim::platform::IApplication {
public:
  Test(int argc, char *argv[]);

private:
  void Init() const;
};
} // namespace apps::test

#endif