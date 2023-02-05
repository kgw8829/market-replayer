#pragma once

#include "messagetype.h"

#include <fstream>
#include <memory>
#include <queue>
#include <vector>

namespace replayer {

const std::string CSV_PATH = "./csv/trade/";

std::string GetCsvName(
    const std::string& symbol,
    const std::string& date)
{
    return CSV_PATH + symbol + "-" + date + ".csv";
}

template <typename TradeCBType>
class DataReader
{
    std::priority_queue<TradeMessage, std::vector<TradeMessage>, CompareTrade> m_pq;
    std::vector<std::string> m_symbols;
    std::vector<std::unique_ptr<std::ifstream>> m_trade_streams;
    TradeCBType m_trade_cb;

public:
    DataReader(
        const std::vector<std::string>& symbols,
        const std::string& date,
        TradeCBType&& on_trade)
    :
        m_symbols{ symbols },
        m_trade_streams(symbols.size()),
        m_trade_cb(std::move(on_trade))
    {
        for (int i = 0; i < symbols.size(); i++) {
            std::string header;
            
            m_trade_streams[i] = std::make_unique<std::ifstream>(GetCsvName(symbols[i], date));
            std::getline(*m_trade_streams[i], header);
        }
    }

    ~DataReader()
    {
        for (auto& trade_stream : m_trade_streams)
            trade_stream->close();
    }

    void Run()
    {
        do
        {
            for (int i = 0; i < m_symbols.size(); i++)
            {
                if (m_trade_streams[i]->eof())
                    continue;
                
                std::string s;
                std::getline(*m_trade_streams[i], s);
                m_pq.emplace(m_symbols[i], s);
            }

            m_trade_cb(m_pq.top());
            m_pq.pop();
        }
        while (!m_pq.empty());
    }
};

} // namespace replayer
