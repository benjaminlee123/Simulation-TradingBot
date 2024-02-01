#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"
#include "HelpCmds.h"

class AdvisorBot
{
public:
  AdvisorBot();
  /** Call this to start the sim */
  void init();

private:
  std::vector<std::string> promptUserInput();
  void processUserInput(std::vector<std::string> userInput);
  // Check if userInput matches any of the commands of advisorBot
  bool validateUserInput(std::string &userInput, std::vector<std::string> &arrayOfCmds);
  bool checkHelpArguements(std::vector<std::string> &inputCommand, std::vector<std::string> &arrayOfCmds);
  void fetchHelpCmdParams(std::vector<std::string> &helpParams);
  void saveAvailableCurrency();
  void listAvailableCurrency();
  void findMinPrice(std::vector<std::string> &inputCommand);
  void findMaxPrice(std::vector<std::string> &inputCommand);
  void findAvgPrice(std::vector<std::string> &inputCommand, std::vector<std::string> &pastTimeFrames);
  void predictWeightedMovingAvg(std::vector<std::string> &inputCommand, std::vector<std::string> &pastTimeFrames);
  void getCurrentTrends(std::vector<std::string> &inputCommand);
  void nextTimeStep();
 
  void notACommandError(std::vector<std::string> &inputCommand);
  void exit(std::vector<std::string>& inputCommand);

  std::vector<std::string> helpParams;
  std::vector<std::string> knownCommands;
  HelpCmds helpCmds;
  std::string currentTime;
  std::vector<std::string> productTypes;
  std::vector<std::string> pastTimeFrames;
  OrderBook orderBook{"20200601.csv"};
  Wallet wallet;
};