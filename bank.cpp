
#include <iostream>
#include <fstream>
#include <queue>
#include "bank.h"
#include "bstree.h"
#include "transaction.h"
#include "account.h"

using namespace std;

Bank::Bank() {
    accounts_ = BSTree();
}

// Takes all strings and turns them into Transaction objects stored in a queue
bool Bank::ReadTransactions(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    char type;
    while (file >> type) {
        if (type == 'O') {
            int account_id, fund_id;
            string last_name, first_name;
            file >> account_id >> fund_id >> last_name >> first_name;
            transactions_.push(Transaction(type, account_id, fund_id, last_name, first_name));
        }
        else if (type == 'D' || type == 'W') {
            int account_id, fund_id, amount;
            file >> account_id >> fund_id >> amount;
            transactions_.push(Transaction(type, account_id, fund_id, amount));
        }
        else if (type == 'T') {
            int source_id, source_fund, dest_id, dest_fund, amount;
            file >> source_id >> source_fund >> dest_id >> dest_fund >> amount;
            transactions_.push(Transaction(type, source_id, source_fund, dest_id, dest_fund, amount));
        }
        else if (type == 'H') {
            int account_id;
            file >> account_id;
            transactions_.push(Transaction(type, account_id));
        }
        else if (type == 'F') {
            int account_id, fund_id;
            file >> account_id >> fund_id;
            transactions_.push(Transaction(type, account_id, fund_id));
        }
        else {
            cerr << "ERROR: Unknown Transaction Type" << endl;
        }
    }
    file.close();
    return true;
}

// Processes all Transaction objects stored in queue into a BSTree
void Bank::ProcessTransactions() {
    while (!transactions_.empty()) {
        Transaction transaction = transactions_.front();
        transactions_.pop();
        if (transaction.type() == 'O') {
            Account* account;
            if (!accounts_.Retrieve(transaction.account_id(), account)) {
                if (transaction.fund_id() >= 0 && transaction.fund_id() < 10) {
                    accounts_.Insert(new Account(transaction.account_id(), transaction.first_name(), transaction.last_name(), transaction.fund_id()));
                    accounts_.Retrieve(transaction.account_id(), account);
                    account->AddTransaction(transaction);
                    processed_transactions_.push(transaction);
                }
                else {
                    cerr << "ERROR: Fund ID " << transaction.fund_id() << " not found. Transaction refused" << endl;
                }
            }
            else {
                cerr << "ERROR: Account " << transaction.account_id() << " already open. Transaction refused" << endl;
            }
        }
        else if (transaction.type() == 'D') {
            Account* account;
            if (accounts_.Retrieve(transaction.account_id(), account)) {
                if (account->Deposit(transaction.fund_id(), transaction.amount())) {
                    transaction.set_processed(true);
                    account->AddTransaction(transaction);
                    processed_transactions_.push(transaction);
                }
                else {
                    cerr << "ERROR: Unable to Deposit to Account" << endl;
                    transaction.set_processed(false);
                    account->AddTransaction(transaction);
                    processed_transactions_.push(transaction);
                }
            }
            else {
                cerr << "ERROR: Account " << transaction.account_id() << " not found. Deposit refused." << endl;
            }
        }
        else if (transaction.type() == 'W') {
            Account* account;
            if (accounts_.Retrieve(transaction.account_id(), account)) {
                if (transaction.fund_id() >= 0 && transaction.fund_id() <= 9) {
                    if (account->funds_balance(transaction.fund_id()) < transaction.amount()) {
                        if ((transaction.fund_id() == 0 || transaction.fund_id() == 1)
                            && account->funds_balance(0) + account->funds_balance(1) >= transaction.amount()) {
                            if (transaction.fund_id() == 0) {
                                int amount = transaction.amount();
                                amount -= account->funds_balance(0);
                                transaction.set_amount(account->funds_balance(0));
                                Transaction transaction2('W', transaction.account_id(), 1, amount);
                                account->Withdraw(0, transaction.amount());
                                account->Withdraw(1, amount);
                                transaction.set_processed(true);
                                transaction2.set_processed(true);
                                account->AddTransaction(transaction);
                                account->AddTransaction(transaction2);
                                processed_transactions_.push(transaction);
                                processed_transactions_.push(transaction2);
                            }
                            else if (transaction.fund_id() == 1) {
                                int amount = transaction.amount();
                                amount -= account->funds_balance(1);
                                transaction.set_amount(account->funds_balance(1));
                                Transaction transaction2('W', transaction.account_id(), 0, amount);
                                account->Withdraw(1, transaction.amount());
                                account->Withdraw(0, amount);
                                transaction.set_processed(true);
                                transaction2.set_processed(true);
                                account->AddTransaction(transaction);
                                account->AddTransaction(transaction2);
                                processed_transactions_.push(transaction);
                                processed_transactions_.push(transaction2);
                            }
                        }
                        else if ((transaction.fund_id() == 2 || transaction.fund_id() == 3) && account->funds_balance(2) + account->funds_balance(3) >= transaction.amount()) {
                            if (transaction.fund_id() == 2) {
                                int amount = transaction.amount();
                                amount -= account->funds_balance(2);
                                transaction.set_amount(account->funds_balance(2));
                                Transaction transaction2('W', transaction.account_id(), 3, amount);
                                account->Withdraw(2, transaction.amount());
                                account->Withdraw(3, amount);
                                transaction.set_processed(true);
                                transaction2.set_processed(true);
                                account->AddTransaction(transaction);
                                account->AddTransaction(transaction2);
                                processed_transactions_.push(transaction);
                                processed_transactions_.push(transaction2);
                            }
                            else if (transaction.fund_id() == 3) {
                                int amount = transaction.amount();
                                amount -= account->funds_balance(3);
                                transaction.set_amount(account->funds_balance(3));
                                Transaction transaction2('W', transaction.account_id(), 2, amount);
                                account->Withdraw(3, transaction.amount());
                                account->Withdraw(2, amount);
                                transaction.set_processed(true);
                                transaction2.set_processed(true);
                                account->AddTransaction(transaction);
                                account->AddTransaction(transaction2);
                                processed_transactions_.push(transaction);
                                processed_transactions_.push(transaction2);
                            }
                        }
                        else {
                            cerr << "ERROR: Not enough funds to withdraw " << transaction.amount() << " from " << account->first_name() << " " << account->last_name() << " " << account->fund_name(transaction.fund_id()) << endl;
                            transaction.set_processed(false);
                            account->AddTransaction(transaction);
                            processed_transactions_.push(transaction);
                        }
                    }
                    else if (account->Withdraw(transaction.fund_id(), transaction.amount())) {
                        transaction.set_processed(true);
                        account->AddTransaction(transaction);
                        processed_transactions_.push(transaction);
                    }
                    else {
                        cerr << "ERROR: Not enough funds to withdraw " << transaction.amount() << " from " << account->first_name() << " " << account->last_name() << " " << account->fund_name(transaction.fund_id()) << endl;
                        transaction.set_processed(false);
                        account->AddTransaction(transaction);
                        processed_transactions_.push(transaction);
                    }
                }
                else {
                    cerr << "ERROR: Fund ID " << transaction.fund_id() << " not found. Withdrawl refused." << endl;
                }
            }
            else {
                cerr << "ERROR: Account " << transaction.account_id() << " not found. Withdrawl refused." << endl;
            }

        }
        else if (transaction.type() == 'T') {
            Account* account;
            if (transaction.source_id() == transaction.dest_id()) {
                if (accounts_.Retrieve(transaction.source_id(), account)) {
                    if (transaction.source_fund() >= 0 && transaction.source_fund() <= 9 && transaction.dest_fund() >= 0 && transaction.dest_fund() <= 9) {
                        if (account->funds_balance(transaction.source_fund()) < transaction.amount()) {
                            if ((transaction.source_fund() == 0 || transaction.source_fund() == 1) && account->funds_balance(0) + account->funds_balance(1) >= transaction.amount()) {
                                if (transaction.source_fund() == 0) {
                                    int amount = transaction.amount();
                                    amount -= account->funds_balance(0);
                                    transaction.set_amount(account->funds_balance(0));
                                    Transaction transaction2('T', transaction.source_id(), 0, transaction.dest_id(), transaction.dest_fund(), amount);
                                    account->Transfer(0, transaction.dest_fund(), transaction.amount());
                                    account->Transfer(1, transaction.dest_fund(), amount);
                                    transaction.set_processed(true);
                                    transaction2.set_processed(true);
                                    account->AddTransaction(transaction);
                                    account->AddTransaction(transaction2);
                                    processed_transactions_.push(transaction);
                                    processed_transactions_.push(transaction2);
                                }
                                else if (transaction.source_fund() == 1) {
                                    int amount = transaction.amount();
                                    amount -= account->funds_balance(1);
                                    transaction.set_amount(account->funds_balance(1));
                                    Transaction transaction2('T', transaction.source_id(), 1, transaction.dest_id(), transaction.dest_fund(), amount);
                                    account->Transfer(1, transaction.dest_fund(), transaction.amount());
                                    account->Transfer(0, transaction.dest_fund(), amount);
                                    transaction.set_processed(true);
                                    transaction2.set_processed(true);
                                    account->AddTransaction(transaction);
                                    account->AddTransaction(transaction2);
                                    processed_transactions_.push(transaction);
                                    processed_transactions_.push(transaction2);
                                }
                            }
                            else if ((transaction.source_fund() == 2 || transaction.source_fund() == 3) && account->funds_balance(2) + account->funds_balance(3) >= transaction.amount()) {
                                if (transaction.source_fund() == 2) {
                                    int amount = transaction.amount();
                                    amount -= account->funds_balance(2);
                                    transaction.set_amount(account->funds_balance(2));
                                    Transaction transaction2('T', transaction.source_id(), 2, transaction.dest_id(), transaction.dest_fund(), amount);
                                    account->Transfer(2, transaction.dest_fund(), transaction.amount());
                                    account->Transfer(3, transaction.dest_fund(), amount);
                                    transaction.set_processed(true);
                                    transaction2.set_processed(true);
                                    account->AddTransaction(transaction);
                                    account->AddTransaction(transaction2);
                                    processed_transactions_.push(transaction);
                                    processed_transactions_.push(transaction2);
                                }
                                else if (transaction.source_fund() == 3) {
                                    int amount = transaction.amount();
                                    amount -= account->funds_balance(3);
                                    transaction.set_amount(account->funds_balance(3));
                                    Transaction transaction2('T', transaction.source_id(), 3, transaction.dest_id(), transaction.dest_fund(), amount);
                                    account->Transfer(3, transaction.dest_fund(), transaction.amount());
                                    account->Transfer(2, transaction.dest_fund(), amount);
                                    transaction.set_processed(true);
                                    transaction2.set_processed(true);
                                    account->AddTransaction(transaction);
                                    account->AddTransaction(transaction2);
                                    processed_transactions_.push(transaction);
                                    processed_transactions_.push(transaction2);
                                }
                            }
                            else {
                                cerr << "ERROR: Not enough funds to transfer " << transaction.amount() << " from " << account->first_name() << " " << account->last_name() << " " << account->fund_name(transaction.source_fund()) << " to " << account->fund_name(transaction.dest_fund()) << endl;
                                transaction.set_processed(false);
                                account->AddTransaction(transaction);
                                processed_transactions_.push(transaction);
                            }
                        }
                        else if (account->Transfer(transaction.source_fund(), transaction.dest_fund(), transaction.amount())) {
                            transaction.set_processed(true);
                            account->AddTransaction(transaction);
                            processed_transactions_.push(transaction);
                        }
                        else {
                            cerr << "ERROR: Not enough funds to transfer " << transaction.amount() << " from " << account->first_name() << " " << account->last_name() << " " << account->fund_name(transaction.source_fund()) << " to " << account->fund_name(transaction.dest_fund()) << endl;
                            transaction.set_processed(false);
                            account->AddTransaction(transaction);
                            processed_transactions_.push(transaction);
                        }
                    }
                    else if (transaction.source_fund() < 0 || transaction.source_fund() > 9) {
                        cerr << "ERROR: Fund ID " << transaction.source_fund() << " not found. Transferral refused." << endl;
                    }
                    else {
                        cerr << "ERROR: Fund ID " << transaction.dest_fund() << " not found. Transferral refused." << endl;
                    }
                }
                else {
                    cerr << "ERROR: Account " << transaction.source_id() << " not found. Transferral refused." << endl;
                }
            }
            else {
                Account* transfer_account;
                if (accounts_.Retrieve(transaction.source_id(), account) && accounts_.Retrieve(transaction.dest_id(), transfer_account)) {
                    if (transaction.source_fund() >= 0 && transaction.source_fund() <= 9 && transaction.dest_fund() >= 0 && transaction.dest_fund() <= 9) {
                        if (account->funds_balance(transaction.source_fund()) < transaction.amount()) {
                            if ((transaction.source_fund() == 0 || transaction.source_fund() == 1) && account->funds_balance(0) + account->funds_balance(1) >= transaction.amount()) {
                                if (transaction.source_fund() == 0) {
                                    int amount = transaction.amount();
                                    amount -= account->funds_balance(0);
                                    transaction.set_amount(account->funds_balance(0));
                                    Transaction transaction2('T', transaction.source_id(), 0, transaction.dest_id(), transaction.dest_fund(), amount);
                                    account->TransferAccounts(0, transfer_account, transaction.dest_fund(), transaction.amount());
                                    account->TransferAccounts(1, transfer_account, transaction.dest_fund(), amount);
                                    transaction.set_processed(true);
                                    transaction2.set_processed(true);
                                    account->AddTransaction(transaction);
                                    account->AddTransaction(transaction2);
                                    transfer_account->AddTransaction(transaction);
                                    transfer_account->AddTransaction(transaction2);
                                    processed_transactions_.push(transaction);
                                    processed_transactions_.push(transaction2);
                                }
                                else if (transaction.source_fund() == 1) {
                                    int amount = transaction.amount();
                                    amount -= account->funds_balance(1);
                                    transaction.set_amount(account->funds_balance(1));
                                    Transaction transaction2('T', transaction.source_id(), 1, transaction.dest_id(), transaction.dest_fund(), amount);
                                    account->TransferAccounts(1, transfer_account, transaction.dest_fund(), transaction.amount());
                                    account->TransferAccounts(0, transfer_account, transaction.dest_fund(), amount);
                                    transaction.set_processed(true);
                                    transaction2.set_processed(true);
                                    account->AddTransaction(transaction);
                                    account->AddTransaction(transaction2);
                                    transfer_account->AddTransaction(transaction);
                                    transfer_account->AddTransaction(transaction2);
                                    processed_transactions_.push(transaction);
                                    processed_transactions_.push(transaction2);
                                }
                            }
                            else if ((transaction.source_fund() == 2 || transaction.source_fund() == 3) && account->funds_balance(2) + account->funds_balance(3) >= transaction.amount()) {
                                if (transaction.source_fund() == 2) {
                                    int amount = transaction.amount();
                                    amount -= account->funds_balance(2);
                                    transaction.set_amount(account->funds_balance(2));
                                    Transaction transaction2('T', transaction.source_id(), 2, transaction.dest_id(), transaction.dest_fund(), amount);
                                    account->TransferAccounts(2, transfer_account, transaction.dest_fund(), transaction.amount());
                                    account->TransferAccounts(3, transfer_account, transaction.dest_fund(), amount);
                                    transaction.set_processed(true);
                                    transaction2.set_processed(true);
                                    account->AddTransaction(transaction);
                                    account->AddTransaction(transaction2);
                                    transfer_account->AddTransaction(transaction);
                                    transfer_account->AddTransaction(transaction2);
                                    processed_transactions_.push(transaction);
                                    processed_transactions_.push(transaction2);
                                }
                                else if (transaction.source_fund() == 3) {
                                    int amount = transaction.amount();
                                    amount -= account->funds_balance(3);
                                    transaction.set_amount(account->funds_balance(3));
                                    Transaction transaction2('T', transaction.source_id(), 3, transaction.dest_id(), transaction.dest_fund(), amount);
                                    account->TransferAccounts(3, transfer_account, transaction.dest_fund(), transaction.amount());
                                    account->TransferAccounts(2, transfer_account, transaction.dest_fund(), amount);
                                    transaction.set_processed(true);
                                    transaction2.set_processed(true);
                                    account->AddTransaction(transaction);
                                    account->AddTransaction(transaction2);
                                    transfer_account->AddTransaction(transaction);
                                    transfer_account->AddTransaction(transaction2);
                                    processed_transactions_.push(transaction);
                                    processed_transactions_.push(transaction2);
                                }
                            }
                            else {
                                cerr << "ERROR: Not enough funds to transfer " << transaction.amount() << " from " << account->first_name() << " " << account->last_name() << " " << account->fund_name(transaction.source_fund()) << " to " << transfer_account->first_name() << " " << transfer_account->last_name() << " " << transfer_account->fund_name(transaction.dest_fund()) << endl;
                                transaction.set_processed(false);
                                account->AddTransaction(transaction);
                                transfer_account->AddTransaction(transaction);
                                processed_transactions_.push(transaction);
                            }
                        }
                        else if (account->TransferAccounts(transaction.source_fund(), transfer_account, transaction.dest_fund(), transaction.amount())) {
                            transaction.set_processed(true);
                            account->AddTransaction(transaction);
                            transfer_account->AddTransaction(transaction);
                            processed_transactions_.push(transaction);
                        }
                        else {
                            cerr << "ERROR: Not enough funds to transfer " << transaction.amount() << " from " << account->first_name() << " " << account->last_name() << " " << account->fund_name(transaction.source_fund()) << " to " << transfer_account->first_name() << " " << transfer_account->last_name() << " " << transfer_account->fund_name(transaction.dest_fund()) << endl;
                            transaction.set_processed(false);
                            account->AddTransaction(transaction);
                            transfer_account->AddTransaction(transaction);
                            processed_transactions_.push(transaction);
                        }
                    }
                    else if (transaction.source_fund() < 0 || transaction.source_fund() > 9) {
                        cerr << "ERROR: Fund ID " << transaction.source_fund() << " not found. Transferral refused." << endl;
                    }
                    else {
                        cerr << "ERROR: Fund ID " << transaction.dest_fund() << " not found. Transferral refused." << endl;
                    }
                }
                else if (!accounts_.Retrieve(transaction.source_id(),account)) {
                    cerr << "ERROR: Account " << transaction.source_id() << " not found. Transferral refused." << endl;
                }
                else {
                    cerr << "ERROR: Account " << transaction.dest_id() << " not found. Transferral refused." << endl;
                }
            }
        }
        else if (transaction.type() == 'H') {
            Account* account;
            if (accounts_.Retrieve(transaction.account_id(), account)) {
                account->DisplayTransactions();
                account->AddTransaction(transaction);
                processed_transactions_.push(transaction);
            }
            else {
                cerr << "ERROR: Account " << transaction.account_id() << " not found. Display refused." << endl;
            }
        }
        else if (transaction.type() == 'F') {
            Account* account;
            if (accounts_.Retrieve(transaction.account_id(), account)) {
                if (transaction.fund_id() >= 0 && transaction.fund_id() <= 9) {
                    account->DisplayFundTransactions(transaction.fund_id());
                    account->AddTransaction(transaction);
                    processed_transactions_.push(transaction);
                }
                else {
                    cerr << "ERROR: Fund ID " << transaction.fund_id() << " not found. Display refused." << endl;
                }
            }
            else {
                cerr << "ERROR: Account " << transaction.account_id() << " not found. Display refused." << endl;
            }
        }
    }
}

// Displays all Transactions that were processed into the BSTree
void Bank::DisplayTransactions() {
    cout << endl;
    cout << "Account Balances:" << endl << endl;

    accounts_.PrintAll();
}
