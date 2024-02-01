#pragma once

#include <string>

class CurrentTrend
{
public:
  CurrentTrend(std::string _product,
               double _price,
               int noOfListing);

  std::string product;
  double price;
  double noOfListing;
};