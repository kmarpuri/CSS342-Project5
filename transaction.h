
#ifndef CSS342_PROJECT5_TRANSACTION_H
#define CSS342_PROJECT5_TRANSACTION_H

#include <iostream>
#include <string>

using namespace std;

class Transaction {
public:
    // Open
    Transaction(const char &type, const int &account_id, const int &fund, const string &last_name, const string &first_name);
    // Deposit or Withdraw
    Transaction(const char &type, const int &account_id, const int &fund, const int &amount);
    // Transfer
    Transaction(const char &type, const int &source_id, const int &source_fund, const int &dest_id, const int &dest_fund, const int &amount);
    // Display History for Client
    Transaction(const char &type, const int &account_id);
    // Display History for Fund
    Transaction(const char &type, const int &account_id, const int &fund);
    // Copy Constructor
    Transaction(const Transaction &transaction);

    // Getters
    [[nodiscard]] char type() const;
    [[nodiscard]] int account_id() const;
    [[nodiscard]] int fund_id() const;
    [[nodiscard]] int amount() const;
    [[nodiscard]] int source_id() const;
    [[nodiscard]] int source_fund() const;
    [[nodiscard]] int dest_id() const;
    [[nodiscard]] int dest_fund() const;
    [[nodiscard]] string last_name() const;
    [[nodiscard]] string first_name() const;
    [[nodiscard]] bool processed() const;

    // Setters
    void set_amount(int amount);
    void set_processed(bool processed);

    // Output Stream Overload
    friend ostream& operator<<(ostream &os, const Transaction &transaction);

private:
    char type_; // O D W T H F
    int account_id_; // O D W H F
    int fund_id_; // O D W
    int source_id_; // T
    int source_fund_; // T
    int dest_id_; // T
    int dest_fund_; // T
    int amount_; // D W T

    string last_name_; // O
    string first_name_; // O

    bool processed_ = true;
};


#endif //CSS342_PROJECT5_TRANSACTION_H
