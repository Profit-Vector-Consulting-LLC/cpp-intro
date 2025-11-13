#include <chrono>
#include <iostream>
#include <thread>
#include "BarberShop.h" 


//TODO: implement this class completely
//
//
//Also, possible enhancements: 
// - Multiple barbers sharing the same waiting queue.
// - Per-customer confirmation (customer waits until their haircut is done).
// - Randomized haircut times and arrival intervals using std::mt19937 and std::uniform_int_distribution.
// - Simple metrics:
//    - Number of served customers
//    - Number of customers turned away
//    - Average waiting time (store timestamps in an std::unordered_map<CustomerId, time_point>)

BarberShop::BarberShop(int numChairs) {
    // Construct a barber shop with a given number of waiting chairs.
    // Do not create threads here.
    log("Constructor not yet implemented"); 
}

bool BarberShop::enterShop(int customerId) {
    // Called by a customer thread when they attempt to enter the shop.
    // Returns true if the customer sits and waits, false if they leave immediately.
    log("enterShop not yet implemented"); 
    return false;
}

void BarberShop::barberWorkLoop(int barberId) {
    // Main loop executed by a barber thread.
    // The barber keeps serving customers until the shop is closed
    // and there are more waiting customers. He sleeps otherwise
    log("barberWorkLoop not yet implemented.");
}

void BarberShop::closeShop() {
    log("closeShop not yet implemented.");
}

void BarberShop::log(const std::string& message) {
    //TODO: improve with timestamps and with barber/customer id
    std::thread::id threadId = std::this_thread::get_id();
    std::cout << threadId << " : " << message << std::endl;
}

