
#ifndef CONSOLE_H_
#define CONSOLE_H_

#ifdef __linux__
#include <ncurses.h>
#else
#include <Windows.h>
#include "curses.h"
#endif
#include <string>
#include <memory>

// ref : https://stackoverflow.com/questions/22313033/multithreaded-console-i-o

namespace console_input
{

class Console
{
public:
  explicit Console();
  ~Console();

  bool isFinished() const { return m_finished; }
  void inputCheck();

private:
  WINDOW* m_inputLine = nullptr;
  WINDOW* m_outputLine = nullptr;
  bool m_finished = false;

  void sendFormattedMsg(const short& prefixColor, const std::string& prefix, const short& color, const std::string& format, ...);
};

} // console_input

#endif // CONSOLE_H_

