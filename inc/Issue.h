#ifndef ISSUE_H
#define ISSUE_H

#include <iostream>
#include <string>

using namespace std;

template <class T>
class Issue {

private:
    string issueName;
    T value;
    T minValue;
    T maxValue;

public:
    Issue(string iN, T val, T min, T max) : issueName(iN), value(val),
                                            minValue(min), maxValue(max) {}

    string getName()       { return issueName; }

    void setValue(T newValue)   { value = newValue; }
    T getValue()                { return value;     }
    T getMin()                  { return minValue;  }
    T getMax()                  { return maxValue;  }

    void printIssue() {
        cout << "Issue: " << getName() << endl;
        // cout << "Min Value: " << getMin() << endl;
        // cout << "Current Value: " << getValue() << endl;
        // cout << "Max Value: " << getMax() << endl << endl;

    }
};

#endif
