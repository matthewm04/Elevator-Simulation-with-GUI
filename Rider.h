#ifndef Rider_h
#define Rider_h

struct Rider
{
    const int from, to; // index of starting and destination floor, respectively
    const bool goingUp, goingDown;
    Rider(int, int);
    Rider& operator=(const Rider&);
};

#endif
