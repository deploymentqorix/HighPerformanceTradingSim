// Implements: QCSIDM_SRS_002 (IOC/FOK), 027 (Iceberg), 028 (Stop)
#include "order.h"
#include "order_book.h"   // your existing book
#include "md_publisher.h" // to publish trades, tob, l2

bool MatchingEngine::onNewOrder(Order o) {
    // Stop orders park until trigger (028)
    if (o.type == OrderType::StopMarket || o.type == OrderType::StopLimit || o.is_stop) {
        stop_books_[o_symbol(o)].add(std::move(o));   // keep per-symbol container
        return true;
    }

    // Iceberg initialisation (027)
    if (o.isIceberg()) {
        o.visible_qty = std::min(o.display_qty, o.qty);
        o.hidden_qty  = (o.qty > o.visible_qty) ? (o.qty - o.visible_qty) : 0;
        o.leaves      = o.visible_qty;
    } else {
        o.leaves = o.qty;
    }

    // FOK pre-check (002)
    if (o.isFOK()) {
        Qty fillable = book_[o_symbol(o)].simulatedFillableQty(o); // scan opposite side up to price (or best for market)
        if (fillable < o.leaves) {
            reject(o.id, "FOK not fully fillable");  // OE feedback (036)
            return false;
        }
    }

    // match (existing hot path)
    auto res = book_[o_symbol(o)].match(o);  // returns {filled_qty, last_fill_px, residual_inserted}
    publishTrades(res.trades);               // 015
    publishTopOfBook();                      // 016

    // IOC: cancel residual (002)
    if (o.isIOC() && res.residual_inserted) {
        book_[o_symbol(o)].cancelResidual(o.id);
        ackCancel(o.id);                     // 036
    }
    return true;
}
