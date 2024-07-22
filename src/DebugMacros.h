/**
 * \file   DebugMacros.h
 * \author L. Nagy
 * 
 * MIT License
 *
 * Copyright (c) [2016] Lesleis Nagy
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 **/

#ifndef DEBUG_MACROS_H_
#define DEBUG_MACROS_H_

#ifdef DEBUG_MESSAGES

#define MESSAGE_LEVEL_DEBUG  1
#define MESSAGE_LEVEL_INFO   2
#define MESSAGE_LEVEL_WARN   3
#define MESSAGE_LEVEL_ERROR  4
#define MESSAGE_LEVEL_SEVERE 5

#ifndef DEBUG_MESSAGES_LEVEL
#define DEBUG_MESSAGES_LEVEL LEVEL_WARN
#endif

#include <cstring>
#include <iostream>
#include <iomanip>

#define __FILENAME__ ( strrchr(__FILE__, '/')     ?                           \
                       strrchr(__FILE__, '/') + 1 : __FILE__ )

#define DEBUG(msg) do {                                                       \
  if (DEBUG_MESSAGES_LEVEL <= 1) {                                            \
    std::cout << "\e[32m[ \e[32m\e[1mDEBUG\e[0m\e[32m    ]\e[0m"              \
              << "\e[0m \e[32m" << std::left << std::setw(40) << __FILENAME__ \
              << "\e[0m[\e[95m" << std::setw(4)  << __LINE__                  \
              << "\e[0m] : "    << msg << std::endl;                          \
  }                                                                           \
} while(0)

#define INFO(msg)  do {                                                       \
  if (DEBUG_MESSAGES_LEVEL <= 2) {                                            \
    std::cout << "\e[32m[ \e[34m\e[1mINFO \e[0m\e[32m    ]\e[0m"              \
              << "\e[0m \e[34m" << std::setw(40) << __FILENAME__              \
              << "\e[0m[\e[95m" << std::setw(4)  << __LINE__                  \
              << "\e[0m] : "    << msg << std::endl;                          \
  }                                                                           \
} while(0)

#define WARNING(msg) do {                                                     \
  if (DEBUG_MESSAGES_LEVEL <= 3) {                                            \
    std::cout << "\e[32m[ \e[93m\e[1mWARN \e[0m\e[32m    ]\e[0m"              \
              << "\e[0m \e[93m" << std::setw(40) << __FILENAME__              \
              << "\e[0m[\e[95m" << std::setw(4) << __LINE__                   \
              << "\e[0m] : "    << msg << std::endl;                          \
  }                                                                           \
} while(0)

#define ERROR(msg) do {                                                       \
  if (DEBUG_MESSAGES_LEVEL <= 4) {                                            \
    std::cout << "\e[32m[ \e[31m\e[1mERROR \e[0m\e[32m   ]\e[0m"              \
              << "\e[0m \e[31m" << std::setw(40) << __FILENAME__              \
              << "\e[0m[\e[95m" << std::setw(4) << __LINE__                   \
              << "\e[0m] : "    << msg << std::endl;                          \
  }                                                                           \
} while(0)

#define SEVERE(msg) do {                                                      \
  if (DEBUG_MESSAGES_LEVEL <= 5) {                                            \
    std::cout << "\e[32m[ \e[0m\e[41m\e[1mSEVERE \e[0m\e[32m  ]\e[0m"         \
              << "\e[0m \e[31m" << std::setw(40) << __FILENAME__              \
              << "\e[0m[\e[95m" << std::setw(4) << __LINE__                   \
              << "\e[0m] : "    << msg << std::endl;                          \
  }                                                                           \
} while(0)

#else   // DEBUG_MESSAGES

#define DEBUG(...)   do {} while (0)

#define INFO(...)    do {} while (0)

#define WARNING(...) do {} while (0)

#define ERROR(...)   do {} while (0)

#define SEVERE(...)  do {} while (0)

#endif  // DEBUG_MESSAGES

#endif  // DEBUG_MACROS_H_
