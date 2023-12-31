#pragma once

#include "OrderBookEntry.h"
#include <vector>
#include <string>

/** Reads the data CSV and loads it into the order book
 * Note the static classes, no state dependence
*/
class CSVReader
{
    public:

        CSVReader();

        static std::vector<OrderBookEntry> readCSV(std::string csvFile);
        static std::vector<std::string> tokenise(std::string csvLine, char separator);
        static OrderBookEntry stringsToOBE(std::string price,
                                        std::string amount,
                                        std::string timeStamp,
                                        std::string product,
                                        OrderBookType type); 


    private:
        
        static OrderBookEntry stringsToOBE(std::vector<std::string> strings); 
};