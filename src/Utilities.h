/**
 * \file   Utilities.h
 * \author L. Nagy
 *
 * Copyright [2016] Lesleis Nagy. All rights reserved.
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <chrono>

template <typename T>
inline void hash_combine(size_t & seed, const T& v) 
{
  std::hash<T> hasher;
  seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

class Timer
{
public:
  Timer() : mStart(Clock::now()) {}
  
  void reset() 
  {
    mStart = Clock::now();
  }

  double elapsed() const
  {
    std::chrono::time_point<Clock> mEnd = Clock::now();

    return std::chrono::duration_cast<Second> (mEnd - mStart).count(); 
  }

private:
  typedef std::chrono::high_resolution_clock             Clock;
  typedef std::chrono::duration< double, std::ratio<1> > Second;

  std::chrono::time_point<Clock> mStart;
};

#endif  // UTILITIES_H_
