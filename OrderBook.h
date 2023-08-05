#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"

/** Representation of the order book*/
class OrderBook 
{
    public:
        OrderBook(std::string fileName);

        std::vector<std::string> getKnownProducts();

        std::vector<OrderBookEntry> getOrders(OrderBookType type,
                                                std::string product,
                                                std::string timestamp);

        std::string getEarliestTime();

        
        std::string getNextTime(std::string timeStamp);

        void insertOrder(OrderBookEntry& order);
    
        static double getHighPrice(std::vector<OrderBookEntry>& orders);
        static double getLowPrice(std::vector<OrderBookEntry>& orders);

        std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);

        

    private:
        std::vector<OrderBookEntry> orders;

};  