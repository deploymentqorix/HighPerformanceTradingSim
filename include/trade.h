#pragma once
#include <string>

/*
  QCSIDM mapping (trade model):
  - QCSIDM_SRS_015: Trade tape publishing (Trade structure)
  - QCSIDM_SRS_023: Logging of trades (audit uses Trade)
*/

struct Trade {
    int trade_id;
    int buy_order_id;
    int sell_order_id;
    std::string instrument;
    double price;
    int quantity;

    Trade() = default;
    Trade(int tradeId, int buyId, int sellId, const std::string& instr, double p, int q);
};
