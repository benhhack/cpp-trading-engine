// tester for tokenising
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

std::vector<std::string> tokenise(std::string csvLine, char separator)
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




    } while(end != std::string::npos);
    

    return tokens;
}




int main()
{   
    
    // testing
    // std::vector<std::string> tokens;

    // std::string s = "thing1,thing2,thing3";

    // tokens = tokenise(s, ',');

    // for (std::string& t: tokens)
    // {
    //     std::cout << t << std::endl;
    // }


    std::ifstream csvFile{"data.csv"};
    std::string line;
    std::vector<std::string> tokens;

    if (csvFile.is_open())
    {
        std::cout << "File open" << std::endl;

        while (std::getline(csvFile, line))
        {
            std::cout << "read line " << line << std::endl;

            tokens = tokenise(line, ',');
            
            

            

            for (std::string& t : tokens)
            {
                std::cout << t << std::endl;
            }

        }
                

    }
    else 
    {
        std::cout << "Could not open file" << std::endl;
    }
        csvFile.close();

    

    
    return 0;
}