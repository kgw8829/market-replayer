#ifndef DATAREADER_H
#define DATAREADER_H

#include <fstream>
#include <iostream>
#include <exception>
#include <functional>
#include "messagetype.h"

class DataReader {
	long last_timestamp_;
	message::Book book_;
	message::Trade trade_;
	std::ifstream b_in_;
	std::ifstream t_in_;
	std::function<void(message::Book&)> book_callback_;
	std::function<void(message::Trade&)> trade_callback_;

	bool ReadBook() {
		if (b_in_.eof())
			return false;

		std::string line;
		std::getline(b_in_, line);
		
		if (line.empty())
			return false;
		
		book_.Parse(line);
		return true;
	}

	bool ReadTrade() {
		if (t_in_.eof())
			return false;

		std::string line;
		std::getline(t_in_, line);
		
		if (line.empty())
			return false;
		
		trade_.Parse(line);
		return true;
	}

public:
	DataReader(std::string name_b, std::string name_t,
			std::function<void(message::Book&)> b_cb,
			std::function<void(message::Trade&)> t_cb)
	:b_in_(name_b), t_in_(name_t), book_callback_(b_cb), trade_callback_(t_cb)
	{
		if (!b_in_.is_open())
			throw std::runtime_error(name_b + "is not valid file name");

		if (!t_in_.is_open())
			throw std::runtime_error(name_t + "is not valid file name");
	}

	void ReadData() {
		while (true) {
			if (book_.time_ == 0L) {
				if(!ReadBook())
					break;

				if (!ReadTrade())
					break;
			} else if (book_.time_ < trade_.time_ && book_.is_used_) {
				if (!ReadBook())
					break;
			} else {
				if (!ReadTrade())
					break;
			}

			if (book_.time_ < trade_.time_) {
				book_callback_(book_);
				book_.Clear();
			} else {
				trade_callback_(trade_);
				trade_.Clear();
			}
		}
	}
};

#endif
