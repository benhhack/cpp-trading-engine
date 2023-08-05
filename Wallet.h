#pragma once

#include <string>
#include <map>
#include "OrderBook.h"

class Wallet
{
    public:
        Wallet();
        /** Inster wallet*/
        void insertCurrency(std::string type, double amount);

        bool removeCurrency(std::string type, double amount);

        /** Check if the wallet contains this much currency or more*/
        bool containsCurrency(std::string type, double amount);

        /** Checks if wallet can cope wiht bid*/
        bool canFulfillOrder(OrderBookEntry order);

        /**
         * update contents of the wallet
         * assumes order was made by the owner of the wallet
        */
        void processSale(OrderBookEntry& sale);

        /** Generate a str repr*/
        std::string toString();



    private:
        std::map<std::string, double> currencies;


    
};