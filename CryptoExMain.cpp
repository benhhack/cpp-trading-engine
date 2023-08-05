#include "CryptoExMain.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"


/** Constructor */
CryptoExMain::CryptoExMain()
{

}

/** App initalisaion */
void CryptoExMain::init()
{
    int input;
    // starts the app at the earliest time
    currentTime = orderBook.getEarliestTime();

    // gives the user some currency to play around with
    wallet.insertCurrency("BTC", 10);
    
    // cycles until a user quits
    bool run = true;
    while(run) {
        printMenu();
        input = getUserOption();
        run = processUserOption(input);
    }
};

/** Prints a menu for the user */
void CryptoExMain::printMenu()
{
    std::cout << "1: Print help" << std::endl;
    std::cout << "2: Print statistics" << std::endl;
    std::cout << "3: Make an offer" << std::endl;
    std::cout << "4: Make a bid" << std::endl;
    std::cout << "5: Print wallet" << std::endl;
    std::cout << "6: Continue" << std::endl;
    std::cout << "7: Exit" << std::endl;
    std::cout << "====================" << std::endl;    

    // let's let a user know what the time is
    std::cout << "Current time is " <<  currentTime << std::endl;   
}

/** Tells user the point of the app */
void CryptoExMain::printHelp(){
    std::cout << "Help - your aim is to make money. Analyze the market and make bids and offers." << std::endl;
}

/** Shows user the max and min ask for each price pair.
 * Also shows how many asks have been seen.
*/
void CryptoExMain::printMarketStats(){

    // iterate through products in order book
    for (std::string const& p : orderBook.getKnownProducts())
    {
        // print the stats for each order
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, 
                                                                p, currentTime);

        std::cout << "Asks seen: " << entries.size() << std::endl;
        std::cout << "Max ask : " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "Min ask : " << OrderBook::getLowPrice(entries) << std::endl;

    }
}

/** Allows a user to enter an ask.
 * User puts in the currency pair, the price and the amount
*/
void CryptoExMain::enterAsk() {
    std::cout << "Make an ask - empty the amount: product, price, amount, eg ETH/BTC,200,0.5" << std::endl;
    std::string input;

    std::getline(std::cin, input);

    // tokenises user input
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "CryptoExMain::enterAsk Bad input " << input << std::endl;
    }
    else {
        try {
            // create an order book entry
            OrderBookEntry obe = CSVReader::stringsToOBE(
            tokens[1], 
            tokens[2],
            currentTime,
            tokens[0],
            OrderBookType::ask
            );
            obe.username = "simuser"; // just to differentiate from the rest of the dataset

            // checks if ask can be fulfulled based on wallet contents, adds it to the order book if it can
            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good" << std::endl;
                orderBook.insertOrder(obe);

            } else {
                std::cout << "Insufficient funds " << input << std::endl;
            }
        } catch(const std::exception& e) 
        {
            std::cout << "CryptoExMain::enterAsk Bad input: " << input << std::endl; 
        }

    }
    // lets a user know what they typed
    std::cout << "You typed " << input << std::endl;


    }

/** Allows a user to enter an ask.
 * User puts in the currency pair, the price and the amount
*/
void CryptoExMain::enterBid()
{
    std::cout << "Make a bid - enter the amount: product,price,amount, eg: ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    // tokenises user input
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "CryptoExMain::enterBid Bad input! " << input << std::endl;
    }
    else {
    try
    {
        // enters bid into order book
        OrderBookEntry obe = CSVReader::stringsToOBE(
            tokens[1],
            tokens[2],
            currentTime,
            tokens[0],
            OrderBookType::bid
            );
            obe.username = "simuser";

        // checks if ask can be fulfulled based on wallet contents, adds it to the order book if it can
        if (wallet.canFulfillOrder(obe))
        {
            std::cout << "Wallet looks good. " << std::endl;
            orderBook.insertOrder(obe);
        }
        else
        {
            std::cout << "Wallet has insufficient funds. " << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "MerkelMain::enterBid Bad input " << std::endl;
    }
    };
}

/** Prints the contents of a user's wallet*/
void CryptoExMain::printWallet(){
    std::cout << wallet.toString() << std::endl;
    }
    
/** Steps forward in time. */
void CryptoExMain::gotoNextTimestamp(){

    std::cout << "Going to next time frame" << std::endl;

    // computes trades in the order book
    std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids("ETH/BTC", currentTime);
    // prints the number of sales
    std::cout << "Sales: " << sales.size() << std::endl;

    // go through all the executed sales
    for (OrderBookEntry &sale : sales)
    {
        std::cout << "Sale price: " << sale.price << " Amount " << sale.amount << std::endl;
    if (sale.username == "simuser")
    {
        // updates the wallet
        wallet.processSale(sale);
    }
    }
    // moves time after computing sales
    currentTime = orderBook.getNextTime(currentTime);
}

/** Takes in the user's option*/
int CryptoExMain::getUserOption() {
    int userOption = 0;
    std::string line;
    std::cout << "Type 1-7" << std::endl;
    std::getline(std::cin, line);
    try
    {
        userOption = std::stoi(line);
    }
    catch(const std::exception& e)
    {
        std::cout << "CryptoExMain::getUserOption Bad Input: Type 1-6" << std::endl;
    }
    
    
    return userOption;
}

/** Decides what to execute depending on the user's option*/
bool CryptoExMain::processUserOption(int userOption) {
    if (userOption == 0) { // bad input
        std::cout << "CryptoExMain::processUserOption Bad Input: Type 1-7" << std::endl;
        }
    if (userOption == 1) { 
        printHelp();
    }
    if (userOption == 2) { 
        printMarketStats();
    }
    if (userOption == 3) { 
        enterAsk();
    }
    if (userOption == 4) { 
        enterBid();
    }
    if (userOption == 5) { 
        printWallet();
    }
    if (userOption == 6) { 
        gotoNextTimestamp();
    }
    if (userOption == 7) { 
        return false;
    }
    return true;
}
