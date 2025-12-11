
#include "account.h"

using namespace std;

// Account Constructor
Account::Account(const int &account_id, const string &first_name, const string &last_name, const int &fund_bonus) {
    account_id_ = account_id;
    first_name_ = first_name;
    last_name_ = last_name;
    for (int i = 0; i < 10; i++) {
        funds_.push_back(Fund(i));
    }
    if (fund_bonus >= 0 && fund_bonus < 10) {
        funds_[fund_bonus].IncreaseBalance(250);
        return;
    }
    funds_[0].IncreaseBalance(250);
}

// Getters
int Account::account_id() const {
    return account_id_;
}

string Account::first_name() const {
    return first_name_;
}

string Account::last_name() const {
    return last_name_;
}

int Account::funds_balance(int fund_id) const {
    if (fund_id >= 0 && fund_id < 10) {
        return funds_[fund_id].balance();
    }
    return -1;
}

string Account::fund_name(int fund_id) const {
    if (fund_id >= 0 && fund_id < 10) {
        return funds_[fund_id].name();
    }
    return "";
}

vector<Fund> Account::funds() const {
    return funds_;
}

vector<Transaction> Account::transactions() const {
    return transactions_;
}

// Deposit Method
bool Account::Deposit(int fund_id, int amount) {
    if (fund_id < 0 || fund_id > 9 || amount < 0) {
        return false;
    }
    funds_[fund_id].IncreaseBalance(amount);
    return true;
}

// Withdraw Method
bool Account::Withdraw(int fund_id, int amount) {
    if (fund_id < 0 || fund_id > 9 || amount < 0) {
        return false;
    }
    if (funds_[fund_id].balance() < amount) {
        return false;
    }
    funds_[fund_id].DecreaseBalance(amount);
    return true;
}

// Transfer within accounts
bool Account::Transfer(int source_fund_id, int dest_fund_id, int amount) {
    if (source_fund_id < 0 || source_fund_id > 9 || dest_fund_id < 0 || dest_fund_id > 9 || amount < 0) {
        return false;
    }
    if (!Withdraw(source_fund_id, amount)) {
        return false;
    }
    return Deposit(dest_fund_id, amount);
}

// Transfer outside accounts
bool Account::TransferAccounts(int source_fund_id, Account* &dest_account, int dest_fund_id, int amount) {
    if (source_fund_id < 0 || source_fund_id > 9 || dest_fund_id < 0 || dest_fund_id > 9 || amount < 0) {
        return false;
    }
    if (!Withdraw(source_fund_id, amount)) {
        return false;
    }
    return dest_account->Deposit(dest_fund_id, amount);
}

// Add transactions into a vector
void Account::AddTransaction(const Transaction &transaction) {
    transactions_.push_back(transaction);
}

// Displays all transactions for an account
void Account::DisplayTransactions() {
    cout << "Transaction History for " << first_name() << " " << last_name() << endl;
    cout << transactions_[0];
    for (int fund_id = 0; fund_id < 10; fund_id++) {
        cout << funds_[fund_id].name() << ": " << funds_[fund_id].balance() << endl;
        for (int i = 1; i < transactions_.size(); i++) {
            if (transactions_[i].type() == 'D' || transactions_[i].type() == 'W') {
                if (transactions_[i].fund_id() == fund_id) {
                    cout << "    " << transactions_[i];
                }
            }
            if (transactions_[i].type() == 'T') {
                if (transactions_[i].source_fund() == fund_id || transactions_[i].dest_fund() == fund_id) {
                    cout << "    " << transactions_[i];
                }
            }
        }
    }
    cout << endl;
}

// Displays all transactions for a certain fund in account
void Account::DisplayFundTransactions(int fund_id) {
    if (fund_id < 0 || fund_id > 9) {
        return;
    }
    cout << "Transaction History for " << first_name_ << " " << last_name_ << " ";
    cout << funds_[fund_id].name() << ": " << funds_[fund_id].balance() << endl;
    for (int i = 1; i < transactions_.size(); i++) {
        if (transactions_[i].type() == 'D' || transactions_[i].type() == 'W') {
            if (transactions_[i].fund_id() == fund_id) {
                cout << "    " << transactions_[i];
            }
        }
        if (transactions_[i].type() == 'T') {
            if (transactions_[i].source_fund() == fund_id || transactions_[i].dest_fund() == fund_id) {
                cout << "    " << transactions_[i];
            }
        }
    }
    cout << endl;
}
