#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"

/**The entry point for the app.
 * 
 * This class defines the exchange object
*/
class CryptoExMain
{
    public:
        CryptoExMain();
        /** Call this to start the sim*/
        void init();

    private:
        void loadOrderBook();
        void printMenu();      
        void printHelp();
        void printMarketStats();
        void enterAsk() ;
        void enterBid();
        void printWallet();
        void gotoNextTimestamp();
        int getUserOption();
        bool processUserOption(int userOption);

            std::string currentTime;
            // if we get more/different data, this will need to be updated
            OrderBook orderBook{"data.csv"};
            Wallet wallet;

};