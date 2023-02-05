#pragma once

#include <iostream>
#include <cstdint>
#include <ostream>
#include <string>
#include <vector>
#include <sstream>

namespace replayer {

enum class BuySell {
    BUY,
    SELL
};

struct TradeMessage
{
    std::string m_symbol;
    std::uint32_t m_agg_trade_id;
    std::uint32_t m_first_trade_id;
    std::uint32_t m_last_trade_id;
    std::uint32_t m_transact_time;
    double m_price;
    double m_quantity;
    BuySell m_buy_sell;

    TradeMessage(const std::string& symbol, const std::string& s)
    : m_symbol{ symbol }
    {
        int token_cnt = 0;
        std::stringstream ss(s);
        std::string token;

        while (std::getline(ss, token, ','))
        {
            switch (token_cnt++) {
                case 0:
                    m_agg_trade_id = std::stoul(token);
                    break;
                case 1:
                    m_price = std::stod(token);
                    break;
                case 2:
                    m_quantity = std::stod(token);
                    break;
                case 3:
                    m_first_trade_id = std::stoul(token);
                    break;
                case 4:
                    m_last_trade_id = std::stoul(token);
                    break;
                case 5:
                    m_transact_time = std::stoul(token);
                    break;
                case 6:
                    m_buy_sell = token == "false" ? BuySell::BUY : BuySell::SELL;
                default:
                    break;
            }
        }
    }

    friend std::ostream& operator<< (std::ostream& os, const TradeMessage& m)
    {
        const auto side = m.m_buy_sell == BuySell::BUY ? "buy" : "sell";
        
        os << "{\n"
           << "\tsymbol: " << m.m_symbol << ",\n"
           << "\ttransact_time: " << m.m_transact_time << ",\n"
           << "\tagg_trade_id: " << m.m_agg_trade_id << ",\n"
           << "\tfirst_trade_id: " << m.m_first_trade_id << ",\n"
           << "\tlast_trade_id: " << m.m_last_trade_id << ",\n"
           << "\tprice: " << m.m_price << ",\n"
           << "\tquantity: " << m.m_quantity << ",\n"
           << "\tside: " << side << '\n'
           << "}";

        return os;
    }
};

struct CompareTrade {
    bool operator() (const TradeMessage& t1, const TradeMessage& t2)
    {
        return t1.m_transact_time > t2.m_transact_time;
    }
};

} // namespace replayer
