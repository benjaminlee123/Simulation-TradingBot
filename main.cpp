#include <iostream>
#include <string>
#include <vector>
#include "OrderBookEntry.h"
#include "MerkelMain.h"
#include "AdvisorBot.h"
#include "CSVReader.h"

int main()
{
    // Create new instance of MerkelMain and launches it.
    // MerkelMain app{};
    // Create new instance of AdvisorBot and launches it.

    AdvisorBot app{};
    app.init();

}
