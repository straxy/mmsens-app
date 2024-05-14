#include "poller.h"
#include <chrono>
#include <csignal>
#include <filesystem>
#include <iostream>
#include <memory>
#include <thread>

using namespace poller;
namespace fs = std::filesystem;

namespace {
static bool signalReceived = false;
};

class SignalHandler {
public:
  SignalHandler() {}
  ~SignalHandler() {}
  static void setupSignals();

private:
  static void handleSignal(int sig);
};

void SignalHandler::setupSignals() {
  std::signal(SIGINT, SignalHandler::handleSignal);
}

void SignalHandler::handleSignal(__attribute__((unused)) int sig) {
  signalReceived = true;
}

int main(int argc, char *argv[]) {
  std::cout << "Hello World!" << std::endl;

  // check that valid sysfs path is passed
  if ((argc < 2) || (argc > 3) || !fs::is_directory(argv[1])) {
    std::cout << "Sysfs path to mmsensX must be passed" << std::endl;
    exit(-1);
  }

  std::unique_ptr<Poller> daemon;
  // Configure signal handling
  SignalHandler::setupSignals();

  // Create a poller
  if (argc == 2) {
    daemon = std::unique_ptr<Poller>(new Poller(argv[1]));
  } else {
    daemon = std::unique_ptr<Poller>(new Poller(argv[1], argv[2]));
  }

  // Initialize the device
  daemon->init();

  // Start poller thread
  daemon->runThread();

  while (!signalReceived) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  // Stop and join thread
  daemon->stopThread();

  daemon->deinit();

  return 0;
}
