#include "CryptoExMain.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"



CryptoExMain::CryptoExMain()
{

}

void CryptoExMain::init()
{
    int input;
    currentTime = orderBook.getEarliestTime();

    wallet.insertCurrency("BTC", 10);

    while(true) {
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
};

void CryptoExMain::printMenu()
{
    std::cout << "1: Print help" << std::endl;
    std::cout << "2: Print statistics" << std::endl;
    std::cout << "3: Make an offer" << std::endl;
    std::cout << "4: Make a bid" << std::endl;
    std::cout << "5: Print wallet" << std::endl;
    // add a time step
    std::cout << "6: Continue" << std::endl;

    std::cout << "7: Exit" << std::endl;

    std::cout << "====================" << std::endl;    

    std::cout << "Current time is " <<  currentTime << std::endl;   
}

void CryptoExMain::printHelp(){
    std::cout << "Help - your aim is to make money. Analyze the market and make bids and offers." << std::endl;
}

void CryptoExMain::printMarketStats(){

    for (std::string const& p : orderBook.getKnownProducts())
    {
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, 
                                                                p, currentTime);

        std::cout << "Asks seen: " << entries.size() << std::endl;
        std::cout << "Max ask : " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "Min ask : " << OrderBook::getLowPrice(entries) << std::endl;

    }
    // std::cout << "OrderBook contains " << orders.size() << " entries" <<std::endl;

    // unsigned int bids = 0; 
    // unsigned int asks = 0; 

    // for (OrderBookEntry& e : orders) {
    //     if (e.orderType == OrderBookType::ask) asks++;
    //     if (e.orderType == OrderBookType::bid) bids++;
    // }

    // std::cout << "OrderBook asks: " << asks << " bids: " << bids << std::endl;
}

void CryptoExMain::enterAsk() { 
    std::cout << "Make an ask - empty the amount: product, price, amount, eg ETH/BTC,200,0.5" << std::endl;
    std::string input;

    // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "CryptoExMain::enterAsk() Bad input " << input << std::endl;
    }
    else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
            tokens[1], 
            tokens[2],
            currentTime,
            tokens[0],
            OrderBookType::bid
            );
            obe.username = "simuser";
            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good" << std::endl;
                orderBook.insertOrder(obe);

            } else {
                std::cout << "Insufficient funds " << input << std::endl;
            }
        } catch(const std::exception& e) 
        {
            std::cout << "CryptoExMain::enterBid Bad input: " << input << std::endl; 
        }

    }

    std::cout << "You typed " << input << std::endl;


    }

void CryptoExMain::enterBid()
{
  std::cout << "Make a bid - enter the amount: product,price,amount, eg: ETH/BTC,200,0.5" << std::endl;
  std::string input;
  std::getline(std::cin, input);

  std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
  if (tokens.size() != 3)
  {
    std::cout << "MerkelMain::enterBid Bad input! " << input << std::endl;
  }
  else
  {
    try
    {
      OrderBookEntry obe = CSVReader::stringsToOBE(
          tokens[1],
          tokens[2],
          currentTime,
          tokens[0],
          OrderBookType::bid
          );
          obe.username = "simuser";

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

void CryptoExMain::printWallet(){
    std::cout << wallet.toString() << std::endl;
    }
    


void CryptoExMain::gotoNextTimestamp(){

  std::cout << "Going to next time frame" << std::endl;
  std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids("ETH/BTC", currentTime);
  std::cout << "Sales: " << sales.size() << std::endl;
  
  for (OrderBookEntry &sale : sales)
  {
    std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl;
    if (sale.username == "simuser")
    {
      // update the wallet
      wallet.processSale(sale);
    }
  }
  currentTime = orderBook.getNextTime(currentTime);
}


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

void CryptoExMain::processUserOption(int userOption) {
    if (userOption == 0) { // bad input
        std::cout << "Bad Input: Type 1-6" << std::endl;
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
    // if (userOption == 7) { 
    //     return false;
    // }
}
