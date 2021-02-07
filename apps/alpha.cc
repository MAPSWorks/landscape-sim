//
// Created by Ivars Rusbergs in 2021
//
#include "alpha.h"

#include <lsim/base/log.h>
#include <lsim/platform/i_application.h>
#include <lsim/platform/types.h>
// TODO: should come from elsewhere
lsim::platform::Settings user_settings{"Alpha app", 1,
                                       lsim::Size<uint32_t>(1280, 720)};

Alpha::Alpha(int argc, char *argv[])
    : lsim::platform::IApplication(argc, argv, user_settings) {
  lsim::base::Log::Info("alpha application", "initialized");
}