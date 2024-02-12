
#ifndef DATE_HELPER_H_
#define DATE_HELPER_H_

#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>

namespace helper
{

static std::wstring getCurrentDateWStr()
{
  struct tm timeinfo;
  errno_t err = 0;

  auto now = std::chrono::system_clock::now();
  auto now_c = std::chrono::system_clock::to_time_t(now);
  err = localtime_s(&timeinfo, &now_c);
  if (err != 0)
  {
    std::wcerr << "Failed getCurrentDateWStr." << std::endl;
    return L"";
  }

  std::wstringstream ss;
  ss << std::put_time(&timeinfo, L"%Y%m%d_%H%M%S");
  return ss.str();
}

} // helper

#endif // DATE_HELPER_H_
