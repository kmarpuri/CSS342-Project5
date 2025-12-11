
#ifndef CSS342_PROJECT5_ACCOUNT_H
#define CSS342_PROJECT5_ACCOUNT_H

#include <iostream>
#include <vector>

#include "fund.h"
#include "transaction.h"

using namespace std;

class Account {
public:
    // Constructor
    Account(const int &account_id, const string &first_name, const string &last_name, const int &fund_bonus);

    // Getters
    int account_id() const;
    string first_name() const;
    string last_name() const;
    int funds_balance(int fund_id) const;
    vector<Fund> funds() const;\
    string fund_name(int fund_id) const;
    vector<Transaction> transactions() const;

    // Transaction Methods
    bool Deposit(int fund_id, int amount);
    bool Withdraw(int fund_id, int amount);
    bool Transfer(int source_fund_id, int dest_fund_id, int amount);
    bool TransferAccounts(int source_fund_id, Account* &dest_account, int dest_fund_id, int amount);
    void AddTransaction(const Transaction &transaction);

    // Display Methods
    void DisplayTransactions();
    void DisplayFundTransactions(int fund_id);

private:
    int account_id_ = -1;
    string first_name_;
    string last_name_;
    vector<Fund> funds_;
    vector<Transaction> transactions_;
};

#endif //CSS342_PROJECT5_ACCOUNT_H