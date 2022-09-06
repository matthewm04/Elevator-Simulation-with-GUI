// identification comments code block
// Programmer: Matthew Mahoney
// Programmer's ID: 1720167
#include "Rider.h"
#include "Building.h"

// only Rider constructor function
Rider::Rider(int start, int dest)
    :from(start), to(dest), goingUp(Building::floors[to] > Building::floors[from] ? true : false),
    goingDown(Building::floors[to] < Building::floors[from] ? true : false) {}

// assignment operator function
Rider& Rider::operator=(const Rider& other) {
    Rider& host = *this;
    if (this != &other) {
        const_cast<int&>(host.from) = other.from;
        const_cast<int&>(host.to) = other.to;
        const_cast<bool&>(host.goingUp) = other.goingUp;
        const_cast<bool&>(host.goingDown) = other.goingDown;
    }
    return host;
}