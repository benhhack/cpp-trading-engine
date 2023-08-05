#include "CSVReader.h"
#include <fstream>
#include <iostream>


CSVReader::CSVReader()
{

}

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
                OrderBookEntry obe = stringsToOBE(tokenise(line, ','));
                entries.push_back(obe);
            }
                catch(const std::exception& e){
                std::cout << "CSVReader::readCSV Bad Data" <<std::endl;
            }
        } //end of while
    }

    std::cout << "CSVReader:: readCSV read "<< entries.size() << " entries" <<std::endl;


    return entries;
}


std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator)
{
    // stores the tokens
    std::vector<std::string> tokens;

    // delineate position
    signed int start, end;
    std::string token;

    start = csvLine.find_first_not_of(separator, 0);


    do {
        end = csvLine.find_first_of(separator, start);

        if (start == csvLine.length() || start == end) break;

        if (end >= 0) token = csvLine.substr(start, end - start);

        else token = csvLine.substr(start, csvLine.length() - start);

        tokens.push_back(token);

        start = end+1;




    } 
    while(end != std::string::npos);
    

    return tokens;
}


OrderBookEntry CSVReader::stringsToOBE(std::string priceString,
                                        std::string amountString,
                                        std::string timeStamp,
                                        std::string product,
                                        OrderBookType type)
{

    double price, amount;
    try {
        price = std::stod(priceString);
        amount = std::stod(amountString);
        std::cout << price << ":" << amount << std::endl;
    }catch(const std::exception& e){
        std::cout << "CSVReader::stringsToOBE Bad float " << priceString<< ":"<<amountString << std::endl;
        throw;
    }
    OrderBookEntry obe{price, 
                        amount, 
                        timeStamp, 
                        product, 
                        type};

    return obe;
}


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