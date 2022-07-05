#ifndef MESSAGETYPE_H
#define MESSAGETYPE_H

#include <string>
#include <utility>
#include <vector>
#include <sstream>

namespace message {
	vector<string> split(std::string input, char delimiter) {
		std::vector<string> tokens;
		std::stringstream ss(input);
		string temp;

		while (std::getline(ss, temp, delimiter))
			tokens.push_back(temp);

		return tokens;
	}

	struct Base {
		std::string type_;
		Base(string type): type(type_) {}
	};

	struct Book: Message {
		long time_;
		std::string symbol_;
		std::vector<std::pair<double, double>> bids_;
		std::vector<std::pair<double, double>> asks_;
		Book(): Base("book") {}

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

	struct Trade: Message {
		long time_;
		std::string symbol_;
		double price_;
		double quantity_;
		bool is_sell_;
		Trade(): Base("trade") {}

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
