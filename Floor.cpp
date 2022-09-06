#include "Floor.h"
#include "Rider.h"
#include "Panel.h"
#include "Building.h"

#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include <climits>

const char* const Floor::UP = "Up";
const char* const Floor::DOWN = "Down";
int Floor::TOP = INT_MIN; // defined in climits
int Floor::BOTTOM = INT_MAX; // also in climits

Floor::Floor(const int elevation, const char* const name, const char* const label)
    :name(name), label(label), elevation(elevation)
{
    panel.addButton(UP);
    panel.addButton(DOWN);

    if (TOP < elevation) TOP = elevation;
    if (elevation < BOTTOM) BOTTOM = elevation;
}

void Floor::addRider(const Rider& r) {
    if (r.goingUp) {
        panel.press(UP);
        upRiders.push(r);
    }
    if (r.goingDown) {
        panel.press(DOWN);
        downRiders.push(r);
    }
}

Rider Floor::removeUpRider() {
    try {
        if (upRiders.empty())
            throw 101;
        else {
            Rider firstRider = upRiders.front();
            upRiders.pop();
            return firstRider;
        }
    }
    catch (int e) {
        cout << "An exception occurred. upRiders queue empty. Code: " << e << endl;
    }
}

Rider Floor::removeDownRider() {
    try {
        if (downRiders.empty())
            throw 102;
        else {
            Rider firstRider = downRiders.front();
            downRiders.pop();
            return firstRider;
        }
    }
    catch (int e) {
        cout << "An exception occurred. downRiders queue empty. Code: " << e << endl;
    }
}

// Floor stream insertion operator
ostream& operator<<(ostream& out, const Floor& floor)
{
    out.setf(ios::right);
    out.width(2);
    out << floor.label;
    out.width(15);
    out << floor.name;
    out << " at";
    out.width(5);
    out << floor.elevation << "\"";
    out << " Up/Down:";
    out.width(5);
    out << floor.upRiders.size() << "/" << floor.downRiders.size();
    out.width(10);
    out << "Buttons:";
    out << floor.panel;

    return out;
}
