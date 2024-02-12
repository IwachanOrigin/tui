
#ifndef DATE_HELPER_H_
#define DATE_HELPER_H_

#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <ctime>

namespace helper
{

static std::string getCurrentDateStr()
{
  auto now = std::chrono::system_clock::now();
  auto now_t = std::chrono::system_clock::to_time_t(now);
#ifdef _WIN32
  struct tm _tm {};
  auto err = localtime_s(&_tm, &now_t);
  std::ostringstream oss;
  oss << std::put_time(&_tm, "%Y%m%d_%H%M%S");
#else
  auto tm_localTime = *std::localtime(&now_t);
  std::ostringstream oss;
  oss << std::put_time(&tm_localTime, "%Y%m%d_%H%M%S");
#endif

  return oss.str();
}

} // helper

#endif // DATE_HELPER_H_
