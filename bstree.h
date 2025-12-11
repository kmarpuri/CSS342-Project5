
#ifndef CSS342_PROJECT5_BSTREE_H
#define CSS342_PROJECT5_BSTREE_H

#include <iostream>
#include "account.h"

using namespace std;

// Node struct for BSTree
struct Node {
    Account* account_;
    Node* left_;
    Node* right_;
};

class BSTree {
public:
    // Constructors
    BSTree();
    BSTree(const BSTree &tree);

    // Destructors
    ~BSTree();
    void DeleteNode(Node* &node);

    // Insert Method
    bool Insert(Account* account);
    bool Insert(Node* &node, Account* account);

    // Retrieve and Deletion Methods
    bool Retrieve(const int &account_id, Account* &account) const;
    bool Delete(const int &account_id, Account* &account);

    // Copy Overloads
    BSTree& operator=(const BSTree &tree);
    void CopyTree(Node* root, Node* &node);

    // Size Methods
    int Size() const;
    int Size(Node* root) const;

    // Output Overloads
    friend ostream& operator<<(ostream &os, const BSTree &tree);
    void PrintInOrder(ostream& os, Node* root) const;

    // Prints all accounts and all transactions in order
    void PrintAll();
    void PrintAll(Node* node);

private:
    Node* root_;
};

#endif //CSS342_PROJECT5_BSTREE_H