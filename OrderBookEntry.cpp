#include "OrderBookEntry.h"


/**Constructor*/
OrderBookEntry::OrderBookEntry( double _price,
                                double _amount,
                                std::string _timeStamp,
                                std::string _product,
                                OrderBookType _orderType,
                                std::string _username)
:   price(_price),
    amount(_amount),
    timeStamp(_timeStamp),
    product(_product),
    orderType(_orderType),
    username(_username)
{

}

/**Converts a string to an order book type. 
 * 
 * Assumes proper input, unknowns are not dealt with.
*/
OrderBookType OrderBookEntry::strToOBT(std::string s)
{
    if (s=="ask") return OrderBookType::ask; 

    if (s=="bid") return OrderBookType::bid; 

    return OrderBookType::unknown;
}
