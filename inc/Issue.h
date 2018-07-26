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

public:
    Issue(string iN, T val) : issueName(iN), value(val) {}

    string getName()       { return issueName; }

    void setValue(T newValue)   { value = newValue; }
    T getValue()                { return value;     }

    void printIssue() {
        cout << "Issue: " << this->getName() << endl;
        cout << "Value: " << this->getValue() << endl;
    }
};

#endif
