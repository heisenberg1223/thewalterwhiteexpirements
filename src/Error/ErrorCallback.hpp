#pragma once

#ifndef ERRORCALLBACK_HPP
#define ERRORCALLBACK_HPP

#include <iostream>

enum ErrorLevels {
  ERROR_FATAL, ERROR_NORMAL, ERROR_NETWORK_FATAL, ERROR_NETWORK_NORMAL
};

#define ANSI_RED "\x1b[1;31m"
#define ANSI_YELLOW "\033[1;33m"
#define END_ANSI "\x1b[0m"

namespace Error {

  inline static void fError(const char *desc, ErrorLevels errorlvl) noexcept {
    switch (errorlvl) {
      case (ERROR_FATAL):
        std::cerr << ANSI_RED << "FATAL ERROR: -- " << END_ANSI;
        std::cerr << desc << '\n';
        break;
      case (ERROR_NORMAL):
        std::cerr << ANSI_RED << "ERROR: " << END_ANSI;
        std::cerr << desc << '\n';
        break;
      case (ERROR_NETWORK_FATAL):
        std::cerr << ANSI_RED << "FATAL NETWORK ERROR: " << END_ANSI;
        std::cerr << desc << '\n';
        break;
      case (ERROR_NETWORK_NORMAL):
        std::cerr << ANSI_RED << "NETWORK ERROR: " << END_ANSI;
        std::cerr << desc << '\n';
        break;
    }
  }

  inline static void fWarning(const char *desc) noexcept {
    std::cout << ANSI_YELLOW << "WARNING: -- " << END_ANSI;
    std::cout << desc << '\n';
  }

};

#endif

