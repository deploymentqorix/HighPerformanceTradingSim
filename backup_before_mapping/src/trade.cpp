#include "trade.h"

Trade::Trade(int tradeId, int buyId, int sellId, const std::string& instr, double p, int q)
    : trade_id(tradeId),
      buy_order_id(buyId),
      sell_order_id(sellId),
      instrument(instr),
      price(p),
      quantity(q) {}
