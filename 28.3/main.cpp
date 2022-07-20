#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <vector>

std::string cook[5] = {"pizza", "soup", "steak", "salad", "sushi"};


std::mutex dishLine;
std::mutex orderLine;
std::vector<std::string> kitchen;
std::vector<std::string> orders;

int ready = 0;

[[noreturn]] void cooking(){
    while (1) {
        std::this_thread::sleep_for(std::chrono::seconds(std::rand() % 10 + 5));
        dishLine.lock();
        while (orders.size() == 0);
        std::cout << std::endl << orders.front() << " ready";
        kitchen.push_back(orders.front());
        dishLine.unlock();
        orderLine.lock();
        orders.erase(orders.begin());
        orderLine.unlock();
    }
}

[[noreturn]] void ordering(){
    while(1){
        std::this_thread::sleep_for(std::chrono::seconds(std::rand() % 5 + 5));
        orderLine.lock();
        orders.push_back(cook[std::rand() % 5]);
        std::cout << std::endl << "Order for " << orders.back();
        orderLine.unlock();
    }
}

[[noreturn]] void courier(){
    while(1){
        std::this_thread::sleep_for(std::chrono::seconds(30));
        dishLine.lock();
        std::cout << std::endl << kitchen.back() << " delivered";
        kitchen.pop_back();
        ready++;
        dishLine.unlock();
    }
}

int main() {
    std::thread cook(cooking);
    std::thread waiter(ordering);
    std::thread delivery(courier);

    cook.detach();
    waiter.detach();
    delivery.detach();
    while(1) {
        if (ready == 5)
            return 0;
    }

}
