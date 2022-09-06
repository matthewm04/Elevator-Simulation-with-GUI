#include "Elevator.h"
#include "Panel.h"
#include "Building.h"

#include <iostream>
#include <vector>
using namespace std;

#include <cstdlib>

Elevator::Elevator(unsigned int capacity, int speed, int start)
    :CAPACITY(capacity), speed(speed)
{
    direction = IDLE;
    timer = 0;
    atFloorIndex = -1;
    location = Building::floors[start].elevation;

    for (int i = 0; i < Building::FLOORS; i++) { // loop to add each floors label to panel
        panel.addButton(Building::floors[i].label);
    }


}

bool Elevator::closeDoor() {
    if (!isOpen())
        return false;
    else {
        //if statement here for when elevator is set to go up, but no up riders
        if (direction == UP) {
            bool upRiders = false;
            for (int i = 0; i < riders.size(); i++) {
                if (riders[i].goingUp)
                    upRiders = true;
            }
            if (upRiders == false)
                panel.clear(Floor::UP);
        }
        if (direction == DOWN) {
            bool downRiders = false;
            for (int i = 0; i < riders.size(); i++) {
                if (riders[i].goingDown)
                    downRiders = true;
            }
            if (downRiders == false)
                panel.clear(Floor::DOWN);
        }
        atFloorIndex = -1; // door is closed
    }
}

bool Elevator::move() {
    int origLocation = location;
    if (direction == UP) {
        for (int i = 0; i < Building::FLOORS; i++) {
            if (Building::floors[i] <= location)
                continue;
            else if (Building::floors[i] - speed > location)
                continue;
            else if (Building::floors[i].panel.isLit(Floor::UP)) {
                location = Building::floors[i];
                if (location == Building::floors[i])
                    openDoorTo(i);
            }
            else if (panel.isLit(Building::floors[i].label)) {
                location = Building::floors[i];
                if (location == Building::floors[i])
                    openDoorTo(i);
            }
        }
        if (origLocation != location)
            return true;
        else if (Building::floors[Building::FLOORS - 1] - speed <= location) {
            location = Building::floors[Building::FLOORS - 1];
            return false;
        }
        else {
            location += speed;
            //return false;
        }
    }
    if (direction == DOWN) {
        for (int i = 0; i < Building::FLOORS; i++) {
            if (Building::floors[i] >= location)
                continue;
            else if (Building::floors[i] + speed < location)
                continue;
            else if (Building::floors[i].panel.isLit(Floor::DOWN)) {
                location = Building::floors[i];
                if (location == Building::floors[i])
                    openDoorTo(i);
            }
            else if (panel.isLit(Building::floors[i].label)) {
                location = Building::floors[i];
                if (location == Building::floors[i])
                    openDoorTo(i);
            }
        }
        if (origLocation != location)
            return true;
        else if (Building::floors[0] + speed >= location) {
            location = Building::floors[0];
            return false;
        }
        else {
            location -= speed;
            //return false;
        }
    }
}

void Elevator::openDoorTo(int floor) {
    panel.clear(Building::floors[floor].label);
    atFloorIndex = floor;
    resetTimer();
}

void Elevator::board(const Rider& r) {
    riders.push_back(r);
    panel.press(Building::floors[r.to].label);

    if (r.goingUp)
        direction = UP;
    else
        direction = DOWN;

    resetTimer();
}

void Elevator::goIdle() {
    direction = IDLE;
}

void Elevator::setDirectionUp() {
    direction = UP;
}

void Elevator::setDirectionDown() {
    direction = DOWN;
}

bool Elevator::hasRiderForFloor() const {
    if (atFloorIndex == -1)
        return false;
    for (int i = 0; i < riders.size(); i++) {
        if (riders[i].to == atFloorIndex)
            return true;
    }
    
    return false;
}

void Elevator::removeRider() {
    for (int i = 0; i < riders.size(); i++) {
        if (riders[i].to == atFloorIndex) {
            riders.erase(riders.begin() + i);
            panel.clear(Building::floors[atFloorIndex].label);

            resetTimer();
            break;
        }
    }
}

// Elevator stream insertion operator
ostream& operator<<(ostream& out, const Elevator& e)
{
    out.setf(ios::right);
    out << "Elevator at";
    out.width(5);
    out << e.location;

    out.width(12);
    if (e.direction == e.IDLE)
        out << "IDLE";
    if (e.direction == e.UP)
        out << "going UP";
    if (e.direction == e.DOWN)
        out << "going DOWN";

    out.width(5);
    out << e.getNumberOfRiders() << " riders";

    if (e.isOpen())
        out << " door is OPEN|" << e.timer << " ";
    
    out << e.panel;

    return out;
}
