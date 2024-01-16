
#include "console.h"
#include <thread>
#include <chrono>
#include <atomic>
#include <iostream>

namespace ci = console_input;

std::atomic<bool> finished = false;

void anotherThread()
{
  std::cout << "started another thread." << std::endl;
  std::chrono::milliseconds ms(10);
  while(1)
  {
    if (finished)
    {
      break;
    }
    std::this_thread::sleep_for(ms);
  }

  std::cout << "finished another thread." << std::endl;
}

int main(int argc, char* argv[])
{
  ci::Console console;

  std::thread th(anotherThread);
  th.detach();

  std::chrono::milliseconds ms(20);
  while(1)
  {
    console.inputCheck();
    if (console.isFinished())
    {
      finished = true;
      std::this_thread::sleep_for(ms);
      break;
    }
    std::this_thread::sleep_for(ms);
  }
  std::cout << "finished main thread" << std::endl;
  return 0;
}

