#ifndef BARBER_SHOP_H
#define BARBER_SHOP_H

#include <string>

class BarberShop {
public:
    // Construct a barber shop with a given number of waiting chairs.
    explicit BarberShop(int numChairs);

    // Called by a customer thread when they attempt to enter the shop.
    // Returns true if the customer sits and waits, false if they leave immediately.
    bool enterShop(int customerId);

    // Main loop executed by a barber thread.
    // The barber keeps serving customers until the shop is closed
    // and there are no more waiting customers.
    void barberWorkLoop(int barberId);

    // Called when no more customers will arrive.
    // Signals barbers to finish current work and go home.
    void closeShop();

private:
    // Basic configuration (non-STL types are allowed).
    int maxChairs;
    bool shopOpen;

    // STUDENT TODO:
    // Add the necessary STL-based member variables here:
    //  - A queue (or similar structure) to hold waiting customers.
    //  - A mutex to protect shared state.
    //  - One or more condition variables to coordinate barbers and customers.
    //  - Any additional counters/flags needed (served customers, etc.).
    //  - NO THREADS!

    void log(const std::string& message);
};

#endif // BARBER_SHOP_H

