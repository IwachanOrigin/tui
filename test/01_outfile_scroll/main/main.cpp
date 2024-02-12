
#include "console.h"
#include <thread>
#include <chrono>
#include <atomic>

namespace ci = console_input;

std::atomic_bool finishedAnother = false;

void anotherThread()
{
  unsigned int index = 0;
  ci::Console::getInstance().writeLog("started another thread.");
  std::chrono::milliseconds ms(1000);
  while(1)
  {
    if (ci::Console::getInstance().isFinished())
    {
      break;
    }
    std::string anotherOutput = "AnotherTh : " + std::to_string(index++);
    ci::Console::getInstance().writeLog(anotherOutput);
    std::this_thread::sleep_for(ms);
  }
  ci::Console::getInstance().writeLog("finished another thread.");
  finishedAnother = true;
}

int main(int argc, char* argv[])
{
  // Init
  ci::Console::getInstance().writeLog("started main thread.");

  std::thread th(anotherThread);
  th.detach();

  std::chrono::milliseconds ms(20);
  while(1)
  {
    ci::Console::getInstance().inputCheck();
    if (ci::Console::getInstance().isFinished())
    {
      break;
    }
    std::this_thread::sleep_for(ms);
  }

  while (!finishedAnother)
  {
    std::this_thread::sleep_for(ms);
  }

  ci::Console::getInstance().writeLog("finished main thread");
  return 0;
}

