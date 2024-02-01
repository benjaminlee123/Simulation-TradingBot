#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"

class MerkelMain
{
public:
    MerkelMain();
    /** Call this to start the sim */
    void init();

private: /** Private functions only accessible through MerkelMain */
    void printMenu();
    void printHelp();
    void printMarketStats();
    void enterAsk();
    void enterBid();
    void printWallet();
    void nextTimeframe();
    int getUserOption();
    void processUserOption(int userOption);

    std::string currentTime;

    OrderBook orderBook{"UseTest.csv"};

    Wallet wallet;
};