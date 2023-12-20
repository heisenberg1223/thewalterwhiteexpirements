#pragma once

#ifndef ERRORCALLBACK_HPP
#define ERRORCALLBACK_HPP

#include <iostream>

enum ErrorLevels {
  ERROR_FATAL, ERROR_NORMAL
};

#define ANSI_RED "\x1b[1;31m"
#define ANSI_YELLOW "\033[1;33m"
#define END_ANSI "\x1b[0m"

namespace Error {

  template<typename Tx>
  inline static void fError(Tx desc, ErrorLevels errorlvl) noexcept {
    if (errorlvl == ERROR_FATAL) {
      std::cerr << ANSI_RED << "FATAL ERROR: -- " << END_ANSI;
      std::cerr << desc << '\n';
    } else if (errorlvl == ERROR_NORMAL) {
      std::cerr << ANSI_RED << "ERROR: " << END_ANSI;
      std::cerr << desc << '\n';
    }
  }

  template<typename Tx>
  inline static void fWarning(Tx desc) noexcept {
    std::cout << ANSI_YELLOW << "WARNING: -- " << END_ANSI;
    std::cout << desc << '\n';
  }

};

#endif

