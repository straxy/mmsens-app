#include <iostream>
#include <filesystem>
#include <thread>
#include <chrono>
#include <csignal>
#include "poller.h"

using namespace poller;
namespace fs = std::filesystem;

namespace
{
static bool signalReceived = false;
};

class SignalHandler
{
public:
   SignalHandler() {}
   ~SignalHandler() {}
   static void setupSignals();
private:
   static void handleSignal(int sig);
};

void SignalHandler::setupSignals()
{
   std::signal(SIGINT, SignalHandler::handleSignal);
}

void SignalHandler::handleSignal(__attribute__((unused))int sig)
{
   signalReceived = true;
}

int main(int argc, char *argv[])
{
   std::cout << "Hello World!" << std::endl;

   // check that valid sysfs path is passed
   if ((argc != 2) || !fs::is_directory(argv[1]))
   {
      std::cout << "Sysfs path to mmsensX must be passed" << std::endl;
      exit(-1);
   }

   // Configure signal handling
   SignalHandler::setupSignals();

   // Create a poller
   Poller daemon(argv[1]);

   // Initialize the device
   daemon.init();

   // Start poller thread
   daemon.runThread();

   while (!signalReceived)
   {
      std::this_thread::sleep_for(std::chrono::seconds(1));
   }

   // Stop and join thread
   daemon.stopThread();

   daemon.deinit();

   return 0;
}
