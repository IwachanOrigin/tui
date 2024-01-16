
#include "console.h"

namespace ci = console_input;

ci::Console::Console()
{

#ifdef _WIN32
  AllocConsole();
#endif
  initscr();

  int width = 0, height = 0;
  getmaxyx(stdscr, height, width);

  if (has_colors())
  {
    start_color();
    for (int i = 1; i <= COLOR_WHITE; i++)
    {
      init_pair(i, i, COLOR_BLACK);
    }
  }

  m_outputLine = newwin(height - 3, width, 0, 0);
  scrollok(m_outputLine, TRUE);
  box(m_outputLine, 0, 0);
  wrefresh(m_outputLine);

  m_inputLine = newwin(3, width, height - 3, 0);
  scrollok(m_inputLine, TRUE);
  box(m_inputLine, 0, 0);
  wrefresh(m_inputLine);

  leaveok(m_inputLine, TRUE);
  nodelay(m_inputLine, TRUE);
  cbreak();
  noecho();
  keypad(m_inputLine, TRUE);
  //keypad(stdscr, TRUE);
}

ci::Console::~Console()
{
  if (m_inputLine)
  {
    delwin(m_inputLine);
    m_inputLine = nullptr;
  }

  if (m_outputLine)
  {
    delwin(m_outputLine);
    m_outputLine = nullptr;
  }

  endwin();
}

void ci::Console::sendFormattedMsg(const short& prefixColor, const std::string& prefix, const short& color, const std::string& format, ...)
{
  va_list args;
  va_start(args, format.c_str());

  if (has_colors())
  {
    if (!prefix.empty())
    {
      wattron(m_outputLine, A_BOLD | COLOR_PAIR(prefixColor));
      wprintw(m_outputLine, prefix.c_str());
    }

    if (color == COLOR_WHITE)
    {
      wattroff(m_outputLine, A_BOLD);
    }
    wattron(m_outputLine, COLOR_PAIR(color));
    vwprintw(m_outputLine, format.c_str(), args);

    wattroff(m_outputLine, A_BOLD | COLOR_PAIR(color));
  }
  else
  {
    wprintw(m_outputLine, prefix.c_str());
    vwprintw(m_outputLine, format.c_str(), args);
  }
  wprintw(m_outputLine, "\n");

  wrefresh(m_outputLine);
  va_end(args);
}

void ci::Console::inputCheck()
{
  static std::string inputStr = "";
  // erase input window.
  werase(m_inputLine);
  box(m_inputLine, 0, 0);
  wattron(m_inputLine, A_BOLD | COLOR_PAIR(COLOR_WHITE));
  mvwaddstr(m_inputLine, 1, 2, "> ");
  wattroff(m_inputLine, A_BOLD | COLOR_PAIR(COLOR_WHITE));

  mvwaddstr(m_inputLine, 1, 4, inputStr.c_str());
  wrefresh(m_inputLine);

  char c = wgetch(m_inputLine);
  if (c == ERR)
  {
    return;
  }

  switch (c)
  {
    case '\n':
    {
      if (!inputStr.empty())
      {
        this->sendFormattedMsg(COLOR_WHITE, "> ", COLOR_WHITE, inputStr);
        if (inputStr.compare("exit") == 0)
        {
          m_finished = true;
        }
        inputStr.clear();
      }
    }
    break;

    case 8:   // BackSpace key
    case 127: // DEL key
    {
      if (!inputStr.empty())
      {
        inputStr.pop_back();
      }
    }
    break;

    case KEY_UP:
    {
      wscrl(m_outputLine, -1);
    }
    break;

    case KEY_DOWN:
    {
      wscrl(m_outputLine, 1);
    }
    break;

    default:
    {
      inputStr += c;
    }
    break;
  }
}




