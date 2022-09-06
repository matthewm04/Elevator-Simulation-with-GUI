#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include "Panel.h"

int Panel::Button::count = 0; // initialize static count variable to 0

// function to add a button using a string as a parameter
void Panel::addButton(string str) {
    Button b;
    b.label = str;
    b.lit = false;
    buttons.push_back(b);
}

// function to press button
void Panel::press(string str) {
    for (int i = 0; i < buttons.size(); i++) {
        if (buttons[i].label == str) {
            buttons[i].lit = true;
            buttons[i].sequence = ++Button::count;
        }
    }
}

// function to clear button press
void Panel::clear(string str) {
    for (int i = 0; i < buttons.size(); i++) {
        if (buttons[i].label == str)
            buttons[i].lit = false;
    }
}

string Panel::getFirstLit() const {
    vector<Button> litButtons;
    string lowestSeq;
    for (int i = 0; i < buttons.size(); i++) { // loop for gathering all lit buttons
        if (buttons[i].lit)
            litButtons.push_back(buttons[i]);
    }
    if (litButtons.empty()) // first, return empty string if no buttons are lit
        return "";
    else if (litButtons.size() == 1) // second, return first lit button if only one button lit
        return litButtons[0].label;
    else // finally, compare lit buttons sequences to see which is first
        for (int i = 0; i < litButtons.size(); i++) {
            if (litButtons[i].sequence < litButtons[i + 1].sequence)
                lowestSeq = litButtons[i].label;
        }
}

bool Panel::isLit(string str) const {
    for (int i = 0; i < buttons.size(); i++) {
        if (buttons[i].label == str)
            return buttons[i].lit;
    }
}

bool Panel::areAnyLit() const {
    bool anyLit = false;
    for (int i = 0; i < buttons.size(); i++) {
        if (buttons[i].lit)
            anyLit = true;
    }
    return anyLit;
}

// Panel stream insertion operator
ostream& operator<<(ostream& out, const Panel& p) {
    for (int i = 0; i < p.buttons.size(); i++) {
        if (p.isLit(p.buttons[i].label)) {
            out << "[" << p.buttons[i].label << "]";
        }
    }
    return out;
}
