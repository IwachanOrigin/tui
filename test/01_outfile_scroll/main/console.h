
#ifndef CONSOLE_H_
#define CONSOLE_H_

#ifdef __linux__
#include <ncurses.h>
#else
#include <Windows.h>
#include "curses.h"
#endif
#include <string>
#include <mutex>

// ref : https://stackoverflow.com/questions/22313033/multithreaded-console-i-o

namespace console_input
{

class Console
{
public:
  static Console& getInstance();

  bool isFinished() const { return m_finished; }
  void inputCheck();
  void writeLog(const std::string& message);

private:
  explicit Console();
  ~Console();
  explicit Console(const Console&) = delete;
  Console& operator=(const Console&) = delete;
  void showLogs();

  WINDOW* m_inputLine = nullptr;
  WINDOW* m_outputLine = nullptr;
  bool m_finished = false;
  std::mutex m_logMutex;
  int m_logOffset = 0;

  void sendFormattedMsg(const short& prefixColor, const std::string& prefix, const short& color, const std::string& format, ...);
};

} // console_input

#endif // CONSOLE_H_

