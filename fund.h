
#ifndef CSS342_PROJECT5_FUND_H
#define CSS342_PROJECT5_FUND_H

#include <iostream>

using namespace std;

class Fund {
public:
    // Constructor
    Fund(int name);

    // Getters
    int balance() const;
    string name() const;

    // Methods
    void IncreaseBalance(int amount);
    void DecreaseBalance(int amount);

private:
    string name_;
    int balance_;

};


#endif //CSS342_PROJECT5_FUND_H