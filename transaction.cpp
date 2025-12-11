
#include <iostream>
#include "transaction.h"

using namespace std;
// O
Transaction::Transaction(const char &type, const int &account_id, const int &fund, const string &last_name, const string &first_name) {
    type_ = type;
    account_id_ = account_id;
    fund_id_ = fund;
    last_name_ = last_name;
    first_name_ = first_name;

}
// D or W
Transaction::Transaction(const char &type, const int &account_id, const int &fund, const int &amount) {
    type_ = type;
    account_id_ = account_id;
    fund_id_ = fund;
    amount_ = amount;
}
// T
Transaction::Transaction(const char &type, const int &source_id, const int &source_fund, const int &dest_id, const int &dest_fund, const int &amount) {
    type_ = type;
    source_id_ = source_id;
    source_fund_ = source_fund;
    dest_id_ = dest_id;
    dest_fund_ = dest_fund;
    amount_ = amount;
}
// H
Transaction::Transaction(const char &type, const int &account_id) {
    type_ = type;
    account_id_ = account_id;
}
// F
Transaction::Transaction(const char &type, const int &account_id, const int &fund) {
    type_ = type;
    account_id_ = account_id;
    fund_id_ = fund;
}
// Copy
Transaction::Transaction(const Transaction &transaction) {
    type_ = transaction.type_;
    account_id_ = transaction.account_id_;
    fund_id_ = transaction.fund_id_;
    source_id_ = transaction.source_id_;
    source_fund_ = transaction.source_fund_;
    dest_id_ = transaction.dest_id_;
    dest_fund_ = transaction.dest_fund_;
    amount_ = transaction.amount_;
    last_name_ = transaction.last_name_;
    first_name_ = transaction.first_name_;
    processed_ = transaction.processed_;
}

// Getters
char Transaction::type() const {
    return type_;
}

int Transaction::account_id() const {
    return account_id_;
}

int Transaction::fund_id() const {
    return fund_id_;
}

int Transaction::amount() const {
    return amount_;
}

int Transaction::source_id() const {
    return source_id_;
}

int Transaction::source_fund() const {
    return source_fund_;
}

int Transaction::dest_id() const {
    return dest_id_;
}

int Transaction::dest_fund() const {
    return dest_fund_;
}

string Transaction::last_name() const {
    return last_name_;
}

string Transaction::first_name() const {
    return first_name_;
}

bool Transaction::processed() const {
    return processed_;
}

// Set amount
void Transaction::set_amount(int amount) {
    amount_ = amount;
}

// Set processed
void Transaction::set_processed(bool processed) {
    processed_ = processed;
}

// Output Overload
ostream& operator<<(ostream &os, const Transaction &transaction) {
    if (transaction.type_ == 'O') {
        os << "Opening Transaction: " << transaction.type_;
        os << " " << transaction.account_id_;
        os << " " << transaction.fund_id_;
        os << " " << transaction.last_name_;
        os << " " << transaction.first_name_ << endl;
    }
    else if (transaction.type_ == 'D') {
        os << transaction.type_ << " ";
        os << transaction.account_id_ << " ";
        os << transaction.fund_id_ << " ";
        os << transaction.amount_;
        if (!transaction.processed_) {
            os << " " << "(Failed)";
        }
        os << endl;
    }
    else if (transaction.type_ == 'W') {
        os << transaction.type_ << " ";
        os << transaction.account_id_ << " ";
        os << transaction.fund_id_ << " ";
        os <<transaction.amount_;
        if (!transaction.processed_) {
            os << " " << "(Failed)";
        }
        os << endl;
    }
    else if (transaction.type_ == 'T') {
        os << transaction.type_ << " ";
        os << transaction.source_id_ << " ";
        os << transaction.source_fund_ << " ";
        os << transaction.dest_id_ << " ";
        os << transaction.dest_fund_ << " ";
        os << transaction.amount_;
        if (!transaction.processed_) {
            os << " " << "(Failed)";
        }
        os << endl;
    }
    else if (transaction.type_ == 'H') {
        os << "Displaying History for Account" << transaction.account_id_ << endl;

    }
    else if (transaction.type_ == 'F') {
        os << "Displaying History for Fund" << transaction.fund_id_ << endl;
    }
    return os;
}