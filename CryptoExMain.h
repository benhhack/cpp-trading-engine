#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"

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
        void processUserOption(int userOption);

            std::string currentTime;
            OrderBook orderBook{"data.csv"};
            Wallet wallet;

};