#ifndef DATAREADER_H
#define DATAREADER_H

#include <fstream>
#include <iostream>
#include <exception>
#include "messagetype.h"
#include "concurrentqueue.h"

class DataReader() {
	long last_timestamp_;
	message::Book book_;
	message::Trade trade_;
	std::ifstream b_in_;
	std::ifstream t_in_;

	bool ReadBook() {
		if (b_in_.eof())
			return false;

		std::string line;
		std::getline(line, b_in_);
		book_.Parse(line);
		return true;
	}

	bool ReadTrade() {
		if (t_in_.eof())
			return false;

		std::string line;
		std::getline(line, t_in_);
		trade_.Parse(line);
		return true;
	}

public:
	DataReader(std::string name_b, std::string name_t)
	:b_in_(name_b), t_in_(name_t)
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
			}

			if (trade_.time == 0L) {
				if (!ReadTrade())
					break;
			}

			if (book_.time_ < trade_.time_) { // add book empty condition and move to the queue
				// push book to the queue
				if (!ReadBook())
					break;
			} else {
				// push trade to the queue
				if (!ReadTrade())
					break;
			}
		}
	}
};

#endif
