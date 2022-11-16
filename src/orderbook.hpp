#pragma once

#include <queue>
#include <optional>
#include <functional>
#include <iostream>
#include <exception>

template<typename Item>
concept Streamable = requires(std::ostream os, Item item) {
    { os << item };
};

struct Order {
    double price;
    long quantity;
    bool is_buy;

    Order(double price, long quantity, bool is_buy) : price(price), quantity(quantity), is_buy(is_buy) {}

    bool operator==(const Order& o) { 
        return this->price == o.price && 
                this->is_buy == o.is_buy && 
                this->quantity == o.quantity; 
    }

    friend bool operator>(const Order& a, const Order& b) {
        return a.price > b.price;
    }
    friend bool operator<(const Order& a, const Order& b) {
        return a.price < b.price;
    }
    friend std::ostream& operator<<(std::ostream&os, const Order& o) {
        os << (o.is_buy ? "B " : "S ") << o.quantity << "@" << o.price;
        return os;
    }
};

template<typename T>
class useful_pq {
public:
    useful_pq() = default;
    useful_pq(std::function<bool (T, T)> pred) : pred(std::move(pred)) {};

    void push(T elem) {
        container.push_back(elem);
        std::push_heap(container.begin(), container.end(), pred);
    }

    void pop() {
        std::pop_heap(container.begin(), container.end(), pred);
        container.pop_back();
    }

    T top() const {
        if (!container.empty()) {
            return *container.begin();
        }
        throw std::invalid_argument("container is empty");
    }

    bool empty() const {
        return container.empty();
    }
    
    typename std::vector<T>::iterator begin() {
        return container.begin();
    }

    typename std::vector<T>::iterator end() {
        return container.end();
    }

private:
    std::vector<T> container;
    std::function<bool (T, T)> pred;
};

struct Orderbook {
    useful_pq<Order> buys;
    useful_pq<Order> sells;

    Orderbook() {
        buys = useful_pq<Order>([](const Order a, const Order b){ return a < b; });
        sells = useful_pq<Order>([](const Order a, const Order b){ return a > b; });
    }
    void add(const Order order);
};

using matcher = std::function<std::optional<Order> (const Orderbook, const Order)>;
void match(Orderbook& orders, const Order order, const matcher m);
std::optional<Order> limit_match(const Orderbook orders, const Order order);

/* DEBUGGING FUNCTIONS */
template<Streamable T>
void print_queue(useful_pq<T> q) {
    while (!q.empty()) {
        auto e = q.top();
        std::cout << e << "\n";
        q.pop();
    }
}
