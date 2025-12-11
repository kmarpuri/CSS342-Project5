
#ifndef CSS342_PROJECT5_BANK_H
#define CSS342_PROJECT5_BANK_H

#include <queue>
#include <fstream>
#include "bstree.h"
#include "transaction.h"

using namespace std;

class Bank {
public:
    // Constructor
    Bank();

    // Bank Processes
    bool ReadTransactions(const string &filename);
    void ProcessTransactions();
    void DisplayTransactions();

private:
    queue<Transaction> transactions_;
    queue<Transaction> processed_transactions_;
    BSTree accounts_;
};

#endif //CSS342_PROJECT5_BANK_H