
#include "fund.h"

using namespace std;

// Constructor that initializes fund name based on fund id
Fund::Fund(int name) {
    if (name == 0) {
        name_ = "Money Market";
    }
    else if (name == 1) {
        name_ = "Prime Money Market";
    }
    else if (name == 2) {
        name_ = "Long-Term Bond";
    }
    else if (name == 3) {
        name_ = "Short-Term Bond";
    }
    else if (name == 4) {
        name_ = "S+P Index Fund";
    }
    else if (name == 5) {
        name_ = "Value Fund";
    }
    else if (name == 6) {
        name_ = "Growth Equity Fund";
    }
    else if (name == 7) {
        name_ = "Growth Index Fund";
    }
    else if (name == 8) {
        name_ = "Crypto ETF";
    }
    else if (name == 9) {
        name_ = "Precious Metals ETF";
    }
    balance_ = 0;
}

// Getter
int Fund::balance() const {
    return balance_;
}

string Fund::name() const {
    return name_;
}

// Increases Balance if valid amound
void Fund::IncreaseBalance(int amount) {
    if (amount < 0) {
        cerr << "ERROR: Cannot Deposit Negative Amount" << endl;
        return;
    }
    balance_ += amount;
}

// Decreases balance by valid amount
void Fund::DecreaseBalance(int amount) {
    if (balance_ < amount && amount < 0) {
        cerr << "ERROR: Insufficient Funds" << endl;
        return;
    }
    balance_ -= amount;
}
