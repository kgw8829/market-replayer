#include <map>
#include "datareader.h"

struct Crypto;

const double PRICE_THRESHOLD = 1;
std::string book_csv = "./data/book_sorted.csv";
std::string trade_csv = "./data/trade_sorted.csv";
std::vector <std::string> symbols {"BTCUSDT", "ETHUSDT", "LINKUSDT", "AAVEUSDT"};
std::map <std::string, Crypto*> cryptos;

struct Crypto {
	std::string symbol_;
	double bid_price_;
	double ask_price_;
	double bid_quantity_;
	double ask_quantity_;

	explicit Crypto(const std::string& symbol): symbol_(symbol) {}

	void OnBook(message::Book &&message) {
		bid_price_ = message.bids_[0].first;
		bid_quantity_ = message.bids_[0].second;
		ask_price_ = message.asks_[0].first;
		ask_quantity_ = message.asks_[0].second;

		if ((symbol_ == "AAVEUSDT" || symbol_ == "LINKUSDT")) {
			CheckRatio();
		}
	}
	
	void OnTrade(message::Trade &&trade) {
	}

	void CheckRatio() {
		auto aave = cryptos["AAVEUSDT"];
		auto link = cryptos["LINKUSDT"];
	
		if (aave -> bid_price_ < PRICE_THRESHOLD || link -> bid_price_ < PRICE_THRESHOLD)
			return;

		auto ratio_bid = aave -> bid_price_ / link -> ask_price_;
		auto ratio_ask = aave -> ask_price_ / link -> bid_price_;
		
		std::cout << ratio_bid << ", " << ratio_ask << " ["<< aave -> bid_price_ << " ,"<< link -> bid_price_ << "]"<<  std::endl;
	}
};

void OnBook(message::Book& message) {
	auto crypto = cryptos[message.symbol_];
	crypto -> OnBook(std::move(message));
}

void OnTrade(message::Trade& message) {
	auto crypto = cryptos[message.symbol_];
	crypto -> OnTrade(std::move(message));
}

int main() {
	for (auto &symbol : symbols)
		cryptos[symbol] = new Crypto(symbol);

	DataReader datareader(book_csv, trade_csv, &OnBook, &OnTrade);
	datareader.ReadData();
}
