//
// Created by Ivars Rusbergs in 2021
//
#include "test.h"

#include <lsim/base/log.h>
#include <lsim/platform/i_application.h>
#include <lsim/platform/types.h>

namespace apps::test {
// TODO: should come from elsewhere
lsim::platform::Settings user_settings{"Alpha app", 1,
                                       lsim::Size<uint32_t>(1280, 720)};

Test::Test(int argc, char *argv[])
    : lsim::platform::IApplication(argc, argv, user_settings) {
  Init();
  lsim::base::Log::Info("test application", "initialized");
}

void Test::Init() const {

  
}

} // namespace apps::test