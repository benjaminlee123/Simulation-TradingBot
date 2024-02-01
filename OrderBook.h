#pragma once

#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>

class OrderBook
{
public:
  /** construct, reading a csv file */
  OrderBook(std::string filename);

  /** return a vector of all known products in the database */
  std::vector<std::string> getKnownProducts();

  /** return vector of Orders according to the sent filters */
  std::vector<OrderBookEntry> getOrders(OrderBookType type, std::string product, std::string timestamp);

  /** Returns earliest time in orderbook */
  std::string getEarliestTime();

  /** Returns the next sent time in orderbook, if no next timestamp reset to earliest timestamp */
  std::string getNextTime(std::string timestamp);

  void insertOrder(OrderBookEntry &order);

  std::vector<OrderBookEntry> matchAskToBids(std::string product, std::string timestamp);

  static double getHighPrice(std::vector<OrderBookEntry> &orders);
  static double getLowPrice(std::vector<OrderBookEntry> &orders);
  static double getAvgPrice(std::vector<OrderBookEntry> &orders, std::string currentTime);
  static double getWeightedMovingAvg(std::vector<double> &minMax);

private:
  std::vector<OrderBookEntry> orders;
};