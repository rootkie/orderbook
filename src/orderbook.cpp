#include<orderbook.hpp>

void match(Orderbook& orders, const Order order, const matcher m) {
    auto matched = m(orders, order);
    if (matched.has_value()) {
        auto matched_order = matched.value();
        order.is_buy ? orders.sells.pop() : orders.buys.pop();
        auto qty = matched_order.quantity - order.quantity;
        if (qty > 0) {
            Order new_order = matched_order;
            new_order.quantity = qty;
            orders.add(new_order);
        } else if (qty < 0) {
            Order new_order = order;
            new_order.quantity = -qty;
            match(orders, new_order, m);
        }
    } else {
        orders.add(order);
    }
}

std::optional<Order> limit_match(const Orderbook orders, const Order order) {
    if (order.is_buy) {
        if (orders.sells.empty()) {
            return std::nullopt;
        }
        auto candidate = orders.sells.top();
        return candidate.price <= order.price ? std::optional{candidate} : std::nullopt;
    } else {
        if (orders.buys.empty()) {
            return std::nullopt;
        }
        auto candidate = orders.buys.top();
        return candidate.price >= order.price ? std::optional{candidate} : std::nullopt;
    }
}

void Orderbook::add(const Order order) {
    auto& book = order.is_buy ? buys : sells;

    for (auto &cur : book) {
        if (cur.price == order.price) {
            cur.quantity += order.quantity;
            return;
        }
    }
    book.push(order);
}