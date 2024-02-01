#pragma once

#include <string>
#include <map>
#include "OrderBookEntry.h"

class Wallet
{
public:
  Wallet();
  /** insert currency to wallet */
  void insertCurrency(std::string type, double amount);
    /** remove currency to wallet */
  bool removeCurrency(std::string type, double amount);
  /** check if the wallet contains this much currency or more */
  bool containsCurrency(std::string type, double amount);
  /** checks if the wallet can cope with this ask or bid */
  bool canFulfillOrder(OrderBookEntry order);
  /** Updates contents of the wallet, assumes order was made by wallet's owner */
  void processSale(OrderBookEntry& sale);
  
  /** generate a string representation of the wallet */
  std::string toString();

private:
  /** map the type of currency to its amount */
  std::map<std::string, double> currencies;
};