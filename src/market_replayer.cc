#include <map>
#include "datareader.h"

std::string book_csv = "./data/book_sorted.csv";
std::string trade_csv = "./data/trade_sorted.csv";

struct Crypto {
	std::string symbol_;
	
	explicit Crypto(const std::string& symbol): symbol_(symbol) {}

	void OnBook() {
		std::cout << symbol_ << std::endl;
	}
};

std::vector <std::string> symbols {"BTCUSDT", "ETHUSDT", "LINKUSDT", "AAVEUSDT"};
std::map <std::string, Crypto*> cryptos;

void OnBook(message::Book&& message) 
{
	auto crypto = cryptos[message.symbol_];
	crypto -> OnBook();
}

void OnTrade(message::Trade&& message) {
	auto crypto = cryptos[message.symbol_];
}

int main() {
	for (auto &symbol : symbols)
		cryptos[symbol] = new Crypto(symbol);

	DataReader datareader(book_csv, trade_csv, &OnBook, &OnTrade);
	datareader.ReadData();
}
