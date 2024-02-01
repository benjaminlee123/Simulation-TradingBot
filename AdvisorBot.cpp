#include "AdvisorBot.h"
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include "HelpCmds.h"
#include "CurrentTrend.h"
#include <signal.h>
#include <Windows.h>

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cstdlib>

using std::cout;
using std::endl;
using std::string;

AdvisorBot::AdvisorBot()
{
}
void showPopUpMessage(const char* message) {
    MessageBoxA(NULL, message, "", MB_OK | MB_ICONINFORMATION);
    //Pop-up Message box welcome feature
}

void AdvisorBot::init()
{
  showPopUpMessage("Happy 2023!, Welcome to AdvisorBot!");
  string userInput;
  std::vector<string> inputCommand;
  knownCommands = {"help", "prod", "min", "max", "avg", "predict", "time", "step","current","exit"};
  currentTime = orderBook.getEarliestTime();
  saveAvailableCurrency();
  cout << "advisorbot> Please enter a command, or help for a list of commands. " << endl;

  

  while (true)
  {
    inputCommand = promptUserInput();
    processUserInput(inputCommand);
  }
}

std::vector<string> tokenise(string userInput, string delimiter)
{
  std::vector<string> tokens;
  signed int start, end;
  string token;
  start = userInput.find_first_not_of(delimiter, 0);

  cout << endl;
  do
  {
    end = userInput.find_first_of(delimiter, start);
    if (start == userInput.length() || start == end)
      break;
    if (end >= 0)
      token = userInput.substr(start, end - start);
    else
      token = userInput.substr(start, userInput.length() - start);
    tokens.push_back(token);
    cout << "Parsed tokens : " << token << endl;
    start = end + 1;
  } while (end > 0);
  cout << endl;

  return tokens;
}

std::vector<string> AdvisorBot::promptUserInput()
{
  string userInput = "";
  std::vector<string> userCommand;
  string line;

  cout << "advisorbot> ";
  std::getline(std::cin, line);

  try
  {
    userInput = line;
    userCommand = tokenise(userInput, " ");
  }
  catch (const std::exception &e)
  {
      std::cerr << "Error: " << e.what() << std::endl;
  }
  return userCommand;
}

bool AdvisorBot::validateUserInput(string &inputCommand, std::vector<std::string> &cmds)
{
  return std::find(cmds.begin(), cmds.end(), inputCommand) != cmds.end();
}

void AdvisorBot::processUserInput(std::vector<string> inputCommand)
{
  if (inputCommand.empty())
  {
    return;
    cout << "advisorbot> Please enter a command, or help for a list of commands. " << endl;
  }
  // Validates user input as a existing cmd w/ 1 arguement.
  if (validateUserInput(inputCommand[0], knownCommands))
  {
      if (inputCommand[0].compare(knownCommands[0]) == 0 && inputCommand.size() == 1)
      {
          cout << "-Help-" << endl;
          cout << "The available commands are, help, help [cmd], prod, min, max, avg, predict, time, step, current and exit " << endl;
          return;

      }
      // Checks if help command has additional arguements
      if (inputCommand[0].compare(knownCommands[0]) == 0 && inputCommand.size() > 2)
      {
          if (checkHelpArguements(inputCommand, knownCommands))
          {
              fetchHelpCmdParams(helpParams);
              return;
          }

          else
              notACommandError(inputCommand);
      }
      if (inputCommand[0].compare(knownCommands[1]) == 0 && inputCommand.size()==1)
      {
          cout << "-Prod-" << endl;
          cout << "======Retreiving list of available currency for sale======" << endl;
          listAvailableCurrency();
          return;
      }
      // if condition for 'min' : first token is 'min' and only 3 tokens in command : 1:min 2:ETH/BTC 3:OrderBookType
      if (inputCommand[0].compare(knownCommands[2]) == 0 && inputCommand.size() == 3)
      {
          cout << "-Min-" << endl;
          findMinPrice(inputCommand);
          return;
      }
      // if condition for 'max' : first token is 'max' and only 3 tokens in command : 1:max 2:ETH/BTC 3:OrderBookType
      if (inputCommand[0].compare(knownCommands[3]) == 0 && inputCommand.size() == 3)
      {
          cout << "-Max-" << endl;
          findMaxPrice(inputCommand);
          return;
      }
      // if condition for 'avg' : first token is 'avg' and only 4 tokens in command : 1:avg 2:ETH/BTC 3:OrderBookType 4:NoOfTimesteps
      // User can only get avg of past timeframes that are available from the current timeframe in the dataset
      if (inputCommand[0].compare(knownCommands[4]) == 0 && inputCommand.size() == 4)
      {
          cout << "-Avg-" << endl;
          findAvgPrice(inputCommand, pastTimeFrames);
          return;
      }
      if (inputCommand[0].compare(knownCommands[5]) == 0 && inputCommand.size() == 4)
      {
          cout << "-Predict-" << endl;
          predictWeightedMovingAvg(inputCommand, pastTimeFrames);
          return;
      }
      if (inputCommand[0].compare(knownCommands[6]) == 0)
      {
          cout << "-Time-" << endl;
          std::cout << "advisorbot> " << currentTime << std::endl;
          return;
      }
      if (inputCommand[0].compare(knownCommands[7]) == 0)
      {
          cout << "-Step-" << endl;
          nextTimeStep();
          return;
      }

      if (inputCommand[0].compare(knownCommands[8]) == 0)
      {
          cout << "-Current-" << endl;
          getCurrentTrends(inputCommand);
          return;
      
      }
      if (inputCommand[0].compare(knownCommands[9]) == 0)
      {
        exit(inputCommand);
    }
  }
  // Checks if userInput is left empty
  else if (inputCommand[0].empty() || inputCommand[0].find_first_not_of(' ') == std::string::npos)
  {
    return;
    cout << "advisorbot> Please enter a command, or help for a list of commands. " << endl;
  }
  else
  {
    notACommandError(inputCommand);
  }
}

bool AdvisorBot::checkHelpArguements(std::vector<std::string> &inputCommand, std::vector<std::string> &cmds)
{
  // prepares an empty helpParams vector
  helpParams.clear();
  std::vector<string>::iterator it;
  // check if help is the first keyword in the command and that there is only 2 tokens, 1 for help and 1 for the opt-arg1.
  if ("help" == inputCommand.front() && inputCommand.size() == 2)
  {
    // check if command is in the format of help w/ an existing cmd as the following keyword.
    string cmd = inputCommand[1];
    it = std::find(cmds.begin(), cmds.end(), cmd);
    if (it != cmds.end())
    {
      // store help and following arguement keyword in params vector.
      helpParams.push_back(inputCommand[0]);
      helpParams.push_back(inputCommand[1]);
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
  return true;
}

void AdvisorBot::fetchHelpCmdParams(std::vector<string> &helpParams)
{
  // Compare 2nd token in helpParams vector to known commands and return proper syntax statements regarding it.
  if (helpParams[1].compare(knownCommands[0]) == 0)
  {
    helpCmds.getHelpCmdSyntax();
  }
  if (helpParams[1].compare(knownCommands[1]) == 0)
  {
    helpCmds.getProdCmdSyntax();
  }
  if (helpParams[1].compare(knownCommands[2]) == 0)
  {
    helpCmds.getMinCmdSyntax();
  }
  if (helpParams[1].compare(knownCommands[3]) == 0)
  {
    helpCmds.getMaxCmdSyntax();
  }
  if (helpParams[1].compare(knownCommands[4]) == 0)
  {
    helpCmds.getAvgCmdSyntax();
  }
  if (helpParams[1].compare(knownCommands[5]) == 0)
  {
    helpCmds.getPredictCmdSyntax();
  }
  if (helpParams[1].compare(knownCommands[6]) == 0)
  {
    helpCmds.getTimeCmdSyntax();
  }
  if (helpParams[1].compare(knownCommands[7]) == 0)
  {
    helpCmds.getStepCmdSyntax();
  }

}

void AdvisorBot::saveAvailableCurrency()
{
  // getKnownProducts will retreive all available products and store them in productTypes vector.
  for (std::string const &p : orderBook.getKnownProducts())
  {
    productTypes.push_back(p);
  }
}

void AdvisorBot::listAvailableCurrency()
{
  for (int i = 0; i != productTypes.size(); ++i)
  {
    // iterate and print all productTypes, if productType is last in list, omit comma seperator.
    if (&productTypes[i] != &productTypes.back())
    {
      cout << productTypes[i] << ", ";
    }
    else
    {
      cout << productTypes[i] << endl;
    }
  }
}

void AdvisorBot::findMinPrice(std::vector<string> &inputCommand)
{
  string currency = inputCommand[1];
  string type = inputCommand[2];
  std::vector<OrderBookEntry> entries;

  // check if currecy is a valid type
  if (validateUserInput(currency, productTypes) && ((type == "bid") || (type == "ask")))
  {
    if (type == "ask")
      entries = orderBook.getOrders(OrderBookType::ask, currency, currentTime);
    if (type == "bid")
      entries = orderBook.getOrders(OrderBookType::bid, currency, currentTime);

    cout << "advisorbot> The min " << type << " for " << currency << " is " << OrderBook::getLowPrice(entries) << endl;
  }
  else
  {
    cout << "advisorbot> Failed to fetch Min price for " << currency << ", make sure command syntax is correct. For more help, enter 'help min' ." << endl;
    return;
  }
}

void AdvisorBot::findMaxPrice(std::vector<string> &inputCommand)
{
  string currency = inputCommand[1];
  string type = inputCommand[2]; 
  std::vector<OrderBookEntry> entries;

  // check if currecy is a valid type
  if (validateUserInput(currency, productTypes) && ((type == "bid") || (type == "ask")))
  {
    if (type == "ask")
      entries = orderBook.getOrders(OrderBookType::ask, currency, currentTime);
    if (type == "bid")
      entries = orderBook.getOrders(OrderBookType::bid, currency, currentTime);

    cout << "advisorbot> The max " << type << " for " << currency << " is " << OrderBook::getHighPrice(entries) << endl;
  }
  else
  {
    cout << "advisorbot> Failed to fetch Max price for " << currency << ", make sure command syntax is correct. For more help, enter 'help max' ." << endl;
    return;
  }
}

void AdvisorBot::findAvgPrice(std::vector<string> &inputCommand, std::vector<string> &pastTimeFrames)
{
  string currency = inputCommand[1];
  string type = inputCommand[2];

  // convert no of timestep from string to int.
  int noOfTimestep = std::stoi(inputCommand[3]);

  // Store list of timesteps in order of most recent first to earliest in the dataset (backwards).
  std::vector<string> recentTimeSteps(pastTimeFrames.rbegin(), pastTimeFrames.rend());
  std::vector<OrderBookEntry> entries;

  if (noOfTimestep >static_cast<int>(recentTimeSteps.size()))
  {
    // Checks if user specified timestep amount is larger than the size of recentTimeSteps list, if so, pull from earliest point in dataset instead.
    noOfTimestep = recentTimeSteps.size();
    cout << "advisorbot> Bot will only fetch average from " << noOfTimestep
         << " timesteps ago, as timestep amount specified in command predates the earliest time in the dataset. " << endl;
  }
  if (validateUserInput(currency, productTypes) && ((type == "bid") || (type == "ask")))
  {
    if (type == "ask")
    {
      for (int i = 0; i != noOfTimestep; ++i)
      {
        entries = orderBook.getOrders(OrderBookType::ask, currency, recentTimeSteps[i]);
      }
    }
    if (type == "bid")
    {
      for (int i = 0; i != noOfTimestep; ++i)
        entries = orderBook.getOrders(OrderBookType::bid, currency, recentTimeSteps[i]);
    }
    cout << " The average " << currency << " " << type << " price over the last " << noOfTimestep << " timesteps was " << OrderBook::getAvgPrice(entries, currentTime) << endl;
  }
}

void AdvisorBot::predictWeightedMovingAvg(std::vector<string> &inputCommand, std::vector<string> &pastTimeFrames)
{
  string minMax = inputCommand[1];
  string currency = inputCommand[2];
  string type = inputCommand[3];
  std::vector<OrderBookEntry> entries;
std::vector<double> minMaxPrices;

if (validateUserInput(currency, productTypes) && ((type == "bid") || (type == "ask")))
{
    if (type == "ask")
    {
        for (int i = 0; i != pastTimeFrames.size(); ++i)
        {
            entries = orderBook.getOrders(OrderBookType::ask, currency, pastTimeFrames[i]);

            if (minMax == "min")
            {
                cout << "c" << endl;
                minMaxPrices.push_back(OrderBook::getLowPrice(entries));
            }
            else if (minMax == "max")
                minMaxPrices.push_back(OrderBook::getHighPrice(entries));
        }
    }
    if (type == "bid")
    {
        for (int i = 0; i != pastTimeFrames.size(); ++i)
        {
            entries = orderBook.getOrders(OrderBookType::bid, currency, pastTimeFrames[i]);

            if (minMax == "min")
                minMaxPrices.push_back(OrderBook::getLowPrice(entries));
            else if (minMax == "max")
                minMaxPrices.push_back(OrderBook::getHighPrice(entries));
        }
    }
}
cout << "advisorbot> The average " << currency << " " << type << " price over the last " << pastTimeFrames.size()
<< " timesteps was " << OrderBook::getWeightedMovingAvg(minMaxPrices) << endl;
cout << "advisorbot> The next " << type << " " << currency << " price might be "
<< OrderBook::getWeightedMovingAvg(minMaxPrices) << endl;
}

void AdvisorBot::getCurrentTrends(std::vector<std::string>& inputCommand)
{
    if (inputCommand.size() != 3)
    {
        cout << "advisorbot> Not enough arguments for the 'current' command." << endl;
        return;
    }

    string minMax = inputCommand[1];
    string type = inputCommand[2];
    double price = 0;
    std::vector<OrderBookEntry> entries1, entriesTemp;
    std::vector<std::vector<OrderBookEntry> > comparisonList;

    if (((minMax == "min") || (minMax == "max")) && ((type == "bid") || (type == "ask")))
    {
        OrderBookType orderBookType;
        if (type == "bid")
        {
            orderBookType = OrderBookType::bid;
        }
        else
        {
            orderBookType = OrderBookType::ask;
        }

        for (const auto& productType : productTypes)
        {
            entriesTemp = orderBook.getOrders(orderBookType, productType, currentTime);
            cout << "getCurrentTrends::entryTemp.size() = " << entriesTemp.size() << " and product is " << productType << endl;
            comparisonList.push_back(entriesTemp);
        }

        if (comparisonList.empty()) {
            cout << "advisorbot> There are no orders available at the current timestep of " << currentTime << endl;
            return;
        }

        std::sort(comparisonList.begin(), comparisonList.end(),
            [](const std::vector<OrderBookEntry>& a, const std::vector<OrderBookEntry>& b) {
                return a.size() < b.size();
            });

        if (minMax == "min")
        {
            price = OrderBook::getLowPrice(comparisonList.back());
        }
        else
        {
            price = OrderBook::getHighPrice(comparisonList.back());
        }

        cout << "advisorbot> The current product with the most amount of " << type << "s at the current timestep of "
         << currentTime << " is " << comparisonList[0][0].product << " with a " << minMax << " price of " << price << endl;
  }
}


void AdvisorBot::nextTimeStep()
{
  pastTimeFrames.push_back(currentTime);

  currentTime = orderBook.getNextTime(currentTime);
  cout << "advisorbot> now at " << currentTime << endl;
 
}



void AdvisorBot::notACommandError(std::vector<string> &inputCommand)
{
  // If user types in an unrecognized command. Advisorbot will warn the user that their command is invalid.
  cout << "advisorbot> '";
  for (int i = 0; i != inputCommand.size(); ++i)
  {
    if (inputCommand[i] != inputCommand.back())
    {
      cout << inputCommand[i] << " ";
    }
    else
    {
      cout << inputCommand[i];
    }
  }
  cout << "' is not a command,Please Enter with the correct Syntax! " << endl;
}



void AdvisorBot::exit(std::vector<std::string>& inputCommand)//Additional Exit function to close AdvisorBot
{
    if (inputCommand[0] == "exit")
    {
        std::cout << "Thank you for using AdvisorBot! Have a good day!:)" << std::endl;
        std::exit(0);
        
    }

}
