#include <iostream>
#include "bstree.h"
#include "account.h"
#include "transaction.h"
#include "bank.h"

using namespace std;

int main(int argc, char *argv[]) {

    if (argc != 2) {
        cerr << "ERROR: Invalid Number of Arguments" << endl;
        return 1;
    }

    Bank bank;
    string filename = argv  [1];

    if (bank.ReadTransactions(filename)) {
        bank.ProcessTransactions();
        bank.DisplayTransactions();
        return 0;
    }

    return 1;
}