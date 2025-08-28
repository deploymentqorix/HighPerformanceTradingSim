print("My Lua strategy has been loaded!")

local order_id_counter = 5000

-- This function will be called by the C++ engine for every trade
function onTrade(instrument, trade)
    print(string.format("[Lua] Trade Alert for %s: %d shares @ %.2f", instrument, trade.quantity, trade.price))
    
    -- Example logic: place a counter-order
    local new_order = {
        id = order_id_counter,
        instrument = instrument,
        type = 0, -- 0 = Limit
        side = 1, -- 1 = Sell
        price = trade.price + 0.01,
        quantity = 10,
        display_quantity = 10
    }
    
    -- Call back into C++ to submit the new order
    bot:submitOrder(new_order)
    order_id_counter = order_id_counter + 1
end

function onBookUpdate(instrument, depth)
    -- This function is called on market depth changes
    -- print("[Lua] Book update for " .. instrument)
end