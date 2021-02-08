//
// Created by Ivars Rusbergs in 2021
//
#ifndef LSIM_APPS_TEST_H_
#define LSIM_APPS_TEST_H_

#include <lsim/platform/i_application.h>

class Test : public lsim::platform::IApplication {
public:
  Test(int argc, char *argv[]);

private:
};

#endif