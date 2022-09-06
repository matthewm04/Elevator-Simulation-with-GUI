#ifndef Panel_h
#define Panel_h

class Panel {
    struct Button
    {
        string label;
        bool lit;
        int sequence; // the index of the order the button was pressed
        static int count; // counter to count total global num of button presses
    };
    vector<Button> buttons;

public:
    void addButton(string);
    void press(string);
    void clear(string);
    string getFirstLit() const;
    bool isLit(string) const;
    bool areAnyLit() const;
    friend ostream& operator<<(ostream&, const Panel&);
};

#endif
