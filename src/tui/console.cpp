
#include "console.h"
#include "datehelper.h"
#include <fstream>

namespace ci = console_input;

const std::string EXTENTION = ".log";

ci::Console& ci::Console::getInstance()
{
  static Console inst;
  return inst;
}

bool ci::Console::init(const std::string& path, const std::string& name)
{
  m_logfileName = path + name + "_" + helper::getCurrentDateStr() + EXTENTION;
  return true;
}

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
        this->writeLog(inputStr);
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
      m_logOffset = (m_logOffset > 0) ? m_logOffset - 1 : 0;
      this->showLogs();
    }
    break;

    case KEY_DOWN:
    {
      ++m_logOffset;
      this->showLogs();
    }
    break;

    default:
    {
      inputStr += c;
    }
    break;
  }
}

void ci::Console::writeLog(const std::string& message)
{
  {
    std::lock_guard<std::mutex> lock(m_logMutex);
    std::ofstream logfile(m_logfileName, std::ios::app);
    if (logfile)
    {
      logfile << message << std::endl;
      ++m_logLineCount;
    }
  }
  this->showLogs();
}

void ci::Console::showLogs()
{
  {
    std::lock_guard<std::mutex> lock(m_logMutex);
    std::ifstream logfile(m_logfileName);
    std::string line = "";
    int lineCount = 0;
    int maxY = 0, maxX = 0;
    getmaxyx(m_outputLine, maxY, maxX);

    // Auto scroll.
    if (m_logLineCount > maxY - 2)
    {
      ++m_logOffset;
    }

    while (lineCount < m_logOffset && std::getline(logfile, line))
    {
      ++lineCount;
    }

    // Clear window
    werase(m_outputLine);
    // Write box
    box(m_outputLine, 0, 0);

    int currentLine = 1;
    while(std::getline(logfile, line) && currentLine < maxY - 1)
    {
      mvwprintw(m_outputLine, currentLine++, 1, "%s", line.substr(0, maxX - 2).c_str());
    }

    wrefresh(m_outputLine);
  }
}


