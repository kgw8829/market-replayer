#include "datareader.h"

#include <iostream>
#include <stdexcept>
#include <unordered_map>

using namespace replayer;

struct Crypto;

std::vector<std::string> symbols { "BTCUSDT", "ETHUSDT", "LINKUSDT", "AAVEUSDT" };
std::unordered_map<std::string, Crypto*> crypto_map;

struct Crypto
{
    std::string m_symbol;
    
    Crypto(const std::string& symbol)
    : m_symbol{ symbol }
    {}

    void OnTrade(const TradeMessage& message)
    {
        std::cout << message << std::endl;
    }
};

void OnTrade(const TradeMessage& message)
{
    crypto_map[message.m_symbol]->OnTrade(message);
}

int main(int argc, char** argv) // input date format e.g) 2022-12-01
{
    if (argc != 2)
        throw std::runtime_error("invalid date");

    std::string date{ argv[1] };

    for (const auto& symbol : symbols)
        crypto_map[symbol] = new Crypto(symbol);

    DataReader data_reader(symbols, date, &OnTrade);
    data_reader.Run();
}
