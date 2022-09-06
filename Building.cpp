#include "Building.h"
#include "Floor.h"
#include "Elevator.h"

#include <iostream>
using namespace std;

#include <cstdlib>
#include <cmath>

Floor Building::floors[] = 
{
  Floor(0, "Ground Floor", "G"), 
  Floor(100, "Second Floor", "2"),
  Floor(200, "Third Floor", "3"),
  Floor(305, "Fourth Floor", "4"),
  Floor(410, "Fifth Floor", "5")
}; 
const int Building::FLOORS = sizeof(floors) / sizeof(Floor);

Elevator Building::elevators[] =
{
  Elevator(12, 5, 0),
  Elevator(12, 5, 1),
  Elevator(12, 5, 2)
};
const int Building::ELEVATORS = sizeof(elevators) / sizeof(Elevator);

// used by Building::placeNewRiders
void Building::getDifferentInts(int& a, int& b)
{
  do 
  {
    a = rand() % FLOORS; // range is 0 to (FLOORS-1)
    b = rand() % FLOORS; // range is 0 to (FLOORS-1)    
  } while (a == b); // try again if they are the same
}

// used by Building::placeNewRiders
int Building::getPoisson(double avg)
{
  int arrivals = 0;
  double probOfnArrivals = exp(-avg); // requires cmath
  for(double randomValue = (rand() % 1000) / 1000.0; // requires cstdlib
    (randomValue -= probOfnArrivals) > 0.0; 
    probOfnArrivals *= avg / ++arrivals);
  return arrivals;
}

// given an arrival rate, randomly select #of riders to add...
// ...and add them to randomly selected floors with random destinations
void Building::placeNewRiders(double arrivalsPerSecond)
{
  int n = getPoisson(arrivalsPerSecond);
  for (int i = 0; i < n; i++)
  {
    int from, to;
    getDifferentInts(from, to);
    Rider rider(from, to);
    floors[from].addRider(rider);
  }
}

// STEP 1 of 9, "Open their door if there is a rider to disembark"
bool Building::openDoorToDisembarkRider(int e)
{
  if (!elevators[e].isOpen()) // if the door is closed...
    for (int f = 0; f < FLOORS; f++) // check each floor to see if its elevation matches 
      if (floors[f] == elevators[e] && 
          elevators[e].panel.isLit(floors[f].label)) // ...and the elevator's panel has that floor lit
        {
          elevators[e].openDoorTo(f); // then open the door to that floor
          return true; // ...and report that an action was taken
        }
  return false; // if I get this far, no action was taken in this function
}

// function checks all conditions required for rider e to safely disembark and returns true or false depending
bool Building::disembarkRider(int e) {
    if (elevators[e].isOpen() && elevators[e].hasRiderForFloor()) {
        elevators[e].removeRider();
        return true;
    }

    return false;
}

bool Building::boardRider(int e) {
    if (elevators[e].isOpen() && !elevators[e].isFull()) { // checks whether elevator door is open and if there is room on the elevator
        Floor& floor = floors[elevators[e].getFloorIndex()]; // creates reference to the floor the rider is on
        
        if (elevators[e].goingUp() && floor.hasUpRider()) { // gets up-bound rider off of floor and onto elevator
            Rider rider = floor.removeUpRider();
            elevators[e].board(rider);
            return true;
        }
        else if (elevators[e].goingDown() && floor.hasDownRider()) { // gets down-bound rider off of floor and onto elevator
            Rider rider = floor.removeDownRider();
            elevators[e].board(rider);
            return true;
        }
    }
    return false;
}

// makes elevator wait on floor until timer runs out if elevator is open
bool Building::waitingForMoreRiders(int e) {
    if (elevators[e].isOpen() && !elevators[e].isIdle() && !elevators[e].timedOut()) {
        elevators[e].tickTimer();
        return true;
    }
    else
        return false;
}

// makes elevator start to move once it is done waiting and all ready to go
bool Building::doneWaitingMove(int e) {
    // if elevator is closed or idle or it's timer has not gone down all the way
    if (!elevators[e].isOpen() || elevators[e].isIdle() || !elevators[e].timedOut())
        return false;
    else {
        elevators[e].closeDoor(); // close the elevators door 
        elevators[e].move();

        return true;
    }
}

bool Building::moveableMove(int e) {
    if (elevators[e].isIdle() || elevators[e].isOpen())
        return false;
    else {
        return elevators[e].move();
    }
}

// function that finds an idle elevator to move in the direction of where a elevator is requested (up or down)
bool Building::setIdleElevatorInMotion(int e) {
    bool action = false;
    for (int i = 0; i < FLOORS; i++) {
        if (elevators[e] != floors[i])
            continue;
        if (floors[i].panel.getFirstLit() == floors[i].UP) {
            elevators[e].openDoorTo(i);
            elevators[e].setDirectionUp();
            action = true;
        }
        else if (floors[i].panel.getFirstLit() == floors[i].DOWN) {
            elevators[e].openDoorTo(i);
            elevators[e].setDirectionDown();
            action = true;
        }
    }
    return action;
}

bool Building::sendIdleElevatorToCallingFloor(int e) {
    bool action = false;
    if (elevators[e].goingDown() || elevators[e].goingUp())
        return action;
    for (int i = 0; i < FLOORS; i++) {
        if (!floors[i].panel.isLit(floors[i].UP) && !floors[i].panel.isLit(floors[i].DOWN))
            continue;
        if (floors[i] > elevators[e]) {
            elevators[e].setDirectionUp();
            elevators[e].move();
            action = true;
        }
        else if (floors[i] < elevators[e]) {
            elevators[e].setDirectionDown();
            elevators[e].move();
            action = true;
        }
    }
    return action;
}

// the "action function" to perform one action per elevator per time step
void Building::action(double arrivalsPerSecond)
{
  placeNewRiders(arrivalsPerSecond);
  
  // one action per elevator
  for (int e = 0; e < ELEVATORS; e++)
  {
    if (openDoorToDisembarkRider(e)) continue; // step 1 of 9
    if (disembarkRider(e)) continue; // step 2 of 9
    if (boardRider(e)) continue; // step 3 of 9
    if (waitingForMoreRiders(e)) continue; // step 4 of 9
    if (doneWaitingMove(e)) continue; // step 5 of 9
    if (moveableMove(e)) continue; // step 6 of 9
    if (setIdleElevatorInMotion(e)) continue; // step 7 of 9
    if (sendIdleElevatorToCallingFloor(e)) continue; // step 8 of 9
    elevators[e].goIdle(); // step 9 of 9
  }
}
