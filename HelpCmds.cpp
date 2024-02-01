#include "HelpCmds.h"
#include <iostream>

HelpCmds::HelpCmds()
{
}

void HelpCmds::getHelpCmdSyntax()
{
  std::cout << "Lists all available commands of AdvisorBot, also allows an additional arguement of a known command for more information" << std::endl;
  std::cout << "Syntax : help [opt-arg1]" << std::endl;
  std::cout << "For e.g. help or help avg" << std::endl;
}

void HelpCmds::getProdCmdSyntax()
{
  std::cout << "Lists all available products for trade" << std::endl;
  std::cout << "Syntax : prodFor e.g. prod" << std::endl;
  std::cout << "For e.g. prod" << std::endl;
}
void HelpCmds::getMinCmdSyntax()
{
  std::cout << "Finds minimum bid/ask of selected product in the current timestep" << std::endl;
  std::cout << "Syntax : min currency1/currency2 orderBookType" << std::endl;
  std::cout << "For e.g. min ETH/BTC ask" << std::endl;
}
void HelpCmds::getMaxCmdSyntax()
{
  std::cout << "Finds maximum bid/ask of selected product in the current timestep" << std::endl;
  std::cout << "Syntax : max currency1/currency2 orderBookType" << std::endl;
  std::cout << "For e.g. max ETH/BTC ask" << std::endl;
}
void HelpCmds::getAvgCmdSyntax()
{
  std::cout << "Finds the average ask/bid of the selected product over the sent number of timesteps" << std::endl;
  std::cout << "Syntax : avg currency1/currency2 orderBookType timesteps" << std::endl;
  std::cout << "For e.g. avg ETH/BTC ask 10" << std::endl;
}
void HelpCmds::getPredictCmdSyntax()
{
  std::cout << "Predicts the min/max bid/ask of the selected product for the next timestep" << std::endl;
  std::cout << "Syntax : predict min/max currency1/currency2 orderBookType" << std::endl;
  std::cout << "For e.g. predict max ETH/BTC bid" << std::endl;
}
void HelpCmds::getTimeCmdSyntax()
{
  std::cout << "States current timeframe in the dataset" << std::endl;
  std::cout << "Syntax : time" << std::endl;
  std::cout << "For e.g. time" << std::endl;
}
void HelpCmds::getStepCmdSyntax()
{
  std::cout << "Advances to next timestep" << std::endl;
  std::cout << "Syntax : step" << std::endl;
  std::cout << "For e.g. step" << std::endl;
 
}
