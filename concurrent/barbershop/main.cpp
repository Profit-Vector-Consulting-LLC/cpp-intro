#include <chrono>
#include <iostream>
#include <thread>
#include "BarberShop.h"

#define NUM_BARBERS             1
#define NUM_CUSTOMERS          20
#define NUM_CHAIRS              5	


void customerThread(int customerId, int arrivalDelayMs, BarberShop& theShop) {
    // Stagger customer arrivals.
    std::this_thread::sleep_for(std::chrono::milliseconds(arrivalDelayMs));
 
    bool gotIn = theShop.enterShop(customerId);
    if (!gotIn) {
        //TODO: log
        // Customer left because there were no chairs.
        return;
    }
    //TODO: log.

    // In this simple version, once a customer is in the queue,
    // the barber logic takes it from there.
    // A more detailed model could add a per-customer wait for
    // "haircut finished".
}

void barberThread(int barberId, BarberShop& theShop) {
    theShop.barberWorkLoop(barberId);
}

int main() {
    BarberShop theShop(NUM_CHAIRS);

    // Create barber threads (use barberThread above)
    std::cout << "Barber thread creation not implemented." << std::endl;

    // Create customer threads (use customerThread above)
    std::cout << "Customer thread creation not implemented." << std::endl;

    // Sleep
    std::cout << "Sleep not implemented." << std::endl;

    theShop.closeShop();

    // Wait for all threads to quit

    std::cout << "Wait for thread termination not implemented." << std::endl;

    // Done
    std::cout << "End simulation." << std::endl;
    return 0;
} 
