#include "CSVReader.h"
#include <fstream>
#include <iostream>

/** Constructor*/
CSVReader::CSVReader()
{

}

/** Read CSV given a filename*/
std::vector<OrderBookEntry> CSVReader::readCSV(std::string csvFilename)
{
    std::vector<OrderBookEntry> entries;

    std::ifstream csvFile{csvFilename};
    std::string line;

    if (csvFile.is_open())
    {
        while(std::getline(csvFile, line)) 
        {
            try
            {
                // create an order book entry from the tokenised input string
                OrderBookEntry obe = stringsToOBE(tokenise(line, ','));
                entries.push_back(obe);
            }
                catch(const std::exception& e){
                std::cout << "CSVReader::readCSV Bad Data" <<std::endl;
            }
        } 
    }

    std::cout << "CSVReader:: readCSV read "<< entries.size() << " entries" <<std::endl;


    return entries;
}

/** Split a string based on a given separator.
 *  Used quite frequently throughout the code.
 */
std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator)
{
    // stores the tokens
    std::vector<std::string> tokens;

    // delineate position
    signed int start, end;
    std::string token;

    // first char
    start = csvLine.find_first_not_of(separator, 0);


    do {
        // goes to first separator
        end = csvLine.find_first_of(separator, start);

        // checks that start and end can actually make a word/string
        if (start == csvLine.length() || start == end) break;

        // get the substring between separator/start/end for a token
        if (end >= 0) token = csvLine.substr(start, end - start);

        // if end has circled into negative
        else token = csvLine.substr(start, csvLine.length() - start);

        tokens.push_back(token);

        start = end+1;




    } 
    while(end != std::string::npos);
    

    return tokens;
}

/**Converts a bunch of strings into an order book entry*/
OrderBookEntry CSVReader::stringsToOBE(std::string priceString,
                                        std::string amountString,
                                        std::string timeStamp,
                                        std::string product,
                                        OrderBookType type)
{

    double price, amount;
    // numbers into doubles conversion
    try {
        price = std::stod(priceString);
        amount = std::stod(amountString);
        std::cout << price << ":" << amount << std::endl;
    }catch(const std::exception& e){
        std::cout << "CSVReader::stringsToOBE Bad float " << priceString<< ":"<<amountString << std::endl;
        throw;
    }
    // create the order book entry
    OrderBookEntry obe{price, 
                        amount, 
                        timeStamp, 
                        product, 
                        type};

    return obe;
}

/** Overloading the function.
 * Takes a vector of string tokens.
*/
OrderBookEntry CSVReader::stringsToOBE(std::vector<std::string> tokens)
{

    double price, amount;
    if (tokens.size() != 5) // bad
    {
        throw std::exception{};
    }

    // we have 5 tokens

    try {
        price = std::stod(tokens[3]);
        amount = std::stod(tokens[4]);
        std::cout << price << ":" << amount << std::endl;
    }catch(const std::exception& e){
        std::cout << "CSVReader::stringsToOBE Bad float " << tokens[3]<< ":"<<tokens[4] << std::endl;
        throw;
    }

    OrderBookEntry obe{price, 
                        amount, 
                        tokens[0], 
                        tokens[1], 
                        OrderBookEntry::strToOBT(tokens[2])};


    return obe;
}