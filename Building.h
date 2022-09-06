// identification comments code block
// Programmer: Matthew Mahoney
// Programmer's ID: 1720167
#ifndef BUILDING_H
#define BUILDING_H

#include "Floor.h"
#include "Elevator.h"

struct Building
{
  static Floor floors[];
  static Elevator elevators[];
  static const int FLOORS;
  static const int ELEVATORS;

  // helper functions
  static void getDifferentInts(int&, int&);
  static int getPoisson(double);
  static void placeNewRiders(double);
  
  // actions
  static void action(double); // the main "action function"
  static bool openDoorToDisembarkRider(int); // step 1 of 9
  static bool disembarkRider(int); // parameter is an elevator's index
  static bool boardRider(int); // parameter is an elvator's index
  static bool waitingForMoreRiders(int); // parameter is an elevator's index
  static bool doneWaitingMove(int); // parameter is an elevator's index
  static bool moveableMove(int); // parameter is also an elevator's index
  static bool setIdleElevatorInMotion(int); // an elevator's index is the parameter
  static bool sendIdleElevatorToCallingFloor(int); // the parameter is the index of an elevator
};
#endif