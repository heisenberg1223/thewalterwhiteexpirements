#pragma once

#ifndef ERRORCALLBACK_HPP
#define ERRORCALLBACK_HPP

#include <iostream>

enum LogLevels {
  ERROR_FATAL, ERROR_NORMAL, ERROR_NETWORK_FATAL, ERROR_NETWORK_NORMAL
}; // the different log levels

#define ANSI_RED "\x1b[1;31m"
#define ANSI_YELLOW "\033[1;33m"
#define ANSI_GREEN "\033[1;32m"

#define END_ANSI "\x1b[0m"

namespace LOG {

  // this function prints a error to the terminal in red text using ansi excape codes.
  inline static void ERROR(std::string desc, LogLevels errorlvl) noexcept {
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

  // this function prints a yellow warning text to the terminal using ansi escape codes.
  inline static void WARNING(std::string desc) noexcept {
    std::cout << ANSI_YELLOW << "WARNING: -- " << END_ANSI;
    std::cout << desc << '\n';
  }

  // this function prints green text to the terminal using ansi escape codes
  inline static void INFO(std::string desc) noexcept {
    std::cout << ANSI_GREEN << "INFO: -- " << END_ANSI;
    std::cout << desc << '\n';
  }
};

#endif

