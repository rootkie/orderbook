#include <orderbook.hpp>
#include <random>
#include <assert.h>

int randint(int base, int cap) {
    assert(base < cap);
    return (rand() % (cap-base) + base);
}

int main(int argc, char **argv) {
    Orderbook orderbook;
    if (argc < 3) {
        std::cout << "Usage ./orderbook rand_seed num_trade\n";
        return 0;
    }
    srand(atoi(argv[1]));

    for (int i=0; i<atoi(argv[2]); i++) {
        auto price = randint(10,100);
        auto quantity = randint(10, 50);
        auto is_buy = randint(0,10)%2 == 0;
        match(orderbook, Order(price, quantity, is_buy), limit_match);
    }

{
    // match(orderbook, Order{30, 5, true}, limit_match);
    // match(orderbook, Order{31, 5, true}, limit_match);
    // match(orderbook, Order{29, 5, true}, limit_match);
    // match(orderbook, Order{33, 5, false}, limit_match);
    // match(orderbook, Order{32, 5, false}, limit_match);
    // match(orderbook, Order{34, 5, false}, limit_match);
    // match(orderbook, Order{32, 5, true}, limit_match);
    // match(orderbook, Order{32, 10, true}, limit_match);
    // match(orderbook, Order{30, 15, false}, limit_match);
}
    
    print_queue<Order>(orderbook.buys);
    print_queue<Order>(orderbook.sells);
}