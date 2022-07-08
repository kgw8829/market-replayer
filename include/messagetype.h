#ifndef MESSAGETYPE_H
#define MESSAGETYPE_H

#include <string>
#include <utility>
#include <vector>
#include <sstream>

namespace message {
	std::vector<std::string> split(std::string input, char delimiter) {
		std::vector<std::string> tokens;
		std::stringstream ss(input);
		std::string temp;

		while (std::getline(ss, temp, delimiter))
			tokens.push_back(temp);

		return tokens;
	}

	struct Base {
		std::string type_;
		bool is_used_;
		Base(std::string type): type_(type), is_used_(false) {}
	};

	struct Book: public Base {
		long time_;
		std::string symbol_;
		std::vector<std::pair<double, double>> bids_;
		std::vector<std::pair<double, double>> asks_;
		
		Book(): Base("book") {}
		
		Book(const Book& book)
		: Base("book"), time_(book.time_), symbol_(book.symbol_)
		{
			bids_ = book.bids_;
			asks_ = book.asks_;
		}

		Book(Book&& book)
		: Base("book"), time_(book.time_), symbol_(book.symbol_), bids_(std::move(book.bids_)), asks_(std::move(book.asks_))
		{
			book.is_used_ = true;
		}

		Book& operator=(const Book& book) {
			type_ = "book";
			time_ = book.time_;
			symbol_ = book.symbol_;
			bids_ = book.bids_;
			asks_ = book.asks_;
			return *this;
		}

		Book& operator=(Book&& book) {
			type_ = "book";
			time_ = book.time_;
			symbol_ = book.symbol_;
			bids_ = std::move(book.bids_);
			asks_ = std::move(book.asks_);
			book.is_used_ = true;
			return *this;
		}

		~Book() {}

		void Parse(std::string& line) {
			auto tokens = split(line, ',');
			time_ = std::stol(tokens[0]);
			symbol_ = tokens[1];

			for (int i = 1; i < 11; i++) {
				bids_.push_back(std::make_pair(std::stod(tokens[2*i]), std::stod(tokens[2*i+1])));
				asks_.push_back(std::make_pair(std::stod(tokens[2*i + 20]), std::stod(tokens[2*i + 21]))); 
			}
		}
	};

	struct Trade: public Base {
		long time_;
		std::string symbol_;
		double price_;
		double quantity_;
		bool is_sell_;

		Trade(): Base("trade") {}

		Trade(Trade& trade): Base("trade") {
			time_ = trade.time_;
			symbol_ = trade.symbol_;
			price_ = trade.price_;
			quantity_ = trade.quantity_;
			is_sell_ = trade.is_sell_;
			trade.is_used_ = true;
		}

		Trade& operator=(Trade& trade) {
			type_ = "trade";
			time_ = trade.time_;
			symbol_ = trade.symbol_;
			price_ = trade.price_;
			quantity_ = trade.quantity_;
			is_sell_ = trade.is_sell_;
			trade.is_used_ = true;
			return *this;
		}

		void Parse(std::string& line) {
			auto tokens = split(line, ',');
			time_ = std::stol(tokens[0]);
			symbol_ = tokens[1];
			price_ = std::stod(tokens[2]);
			quantity_ = std::stod(tokens[3]);
			is_sell_ = tokens[4] == "true";
		}
	};
}

#endif
