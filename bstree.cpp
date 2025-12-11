
#include "bstree.h"
// Default Constructor
BSTree::BSTree() {
    root_ = nullptr;
}

// Copy Constructor
BSTree::BSTree(const BSTree &tree) {
    root_ = nullptr;
    *this = tree;
}

// Destructor
BSTree::~BSTree() {
    DeleteNode(root_);
}

// Inserts account into BSTree
bool BSTree::Insert(Account *account) {
    if (root_ == nullptr) {
        root_ = new Node();
        root_->account_ = account;
        return true;
    }
    if (account->account_id() == root_->account_->account_id()) {
        return false;
    }
    if (account->account_id() < root_->account_->account_id()) {
        return Insert(root_->left_, account);
    }
    if (account->account_id() > root_->account_->account_id()) {
        return Insert(root_->right_, account);
    }
    return false;
}

// Retrieves account from BSTree
bool BSTree::Retrieve(const int &account_id, Account* &account) const {
    if (root_ == nullptr) {
        account = nullptr;
        return false;
    }
    Node* node = root_;
    while (node != nullptr) {
        if (account_id == node->account_->account_id()) {
            account = node->account_;
            return true;
        }
        if (account_id < node->account_->account_id()) {
            node = node->left_;
        }
        else {
            node = node->right_;
        }
    }
    return false;
}

// Deletes account from BSTree, but isn't used in bank simulation
bool BSTree::Delete(const int &account_id, Account* &account) {
    if (root_ == nullptr) {
        account = nullptr;
        return false;
    }
    if (!Retrieve(account_id, account)) {
        return false;
    }
    Node* node = root_;
    Node* parent = nullptr;
    Node* target = nullptr;
    // If root == account
    if (node->account_->account_id() == account_id) {
        target = node;
        if (target->right_ == nullptr && target->left_ == nullptr) {
            root_ = nullptr;
            delete target;
            return true;
        }
        if (target->right_ != nullptr) {
            parent = node;
            node = target->right_;
            while (node->left_ != nullptr) {
                parent = node;
                node = node->left_;
            }
            parent->left_ = node->right_;
            node->right_ = target->right_;
            node->left_ = target->left_;
            root_ = node;
            delete target;
            return true;
        }
        if (target->left_ != nullptr) {
            parent = node;
            node = target->left_;
            while (node->right_ != nullptr) {
                parent = node;
                node = node->right_;
            }
            parent->right_ = node->left_;
            node->left_ = target->left_;
            node->right_ = target->right_;
            root_ = node;
            delete target;
            return true;
        }
    }
    // If root != account but the account exists
    while (node->account_->account_id() != account_id) {
        parent = node;
        if (account_id < node->account_->account_id()) {
            node = node->left_;
        }
        else {
            node = node->right_;
        }
    }
    target = node;
    if (target->right_ == nullptr && target->left_ == nullptr) {
        if (parent->right_ == target) {
            parent->right_ = nullptr;
        }
        else if (parent->left_ == target) {
            parent->left_ = nullptr;
        }
        delete target;
        return true;
    }
    if (node->right_ != nullptr) {
        parent = node;
        node = node->right_;
        while (node->left_ != nullptr) {
            parent = node;
            node = node->left_;
        }
        parent->left_ = node->right_;
        node->right_ = target->right_;
        node->left_ = target->left_;
        delete target;
        return true;
    }
    if (node->left_ != nullptr) {
        parent = node;
        node = node->left_;
        while (node->right_ != nullptr) {
            parent = node;
            node = node->right_;
        }
        parent->right_ = node->left_;
        node->right_ = target->right_;
        node->left_ = target->left_;
        delete target;
        return true;
    }
    return false;
}

// Used for BSTree Destructor
void BSTree::DeleteNode(Node* &node) {
    if (node == nullptr) {
        return;
    }
    DeleteNode(node->left_);
    DeleteNode(node->right_);
    delete node->account_;
    delete node;
    node = nullptr;
}

// Recursive method to Insert account
bool BSTree::Insert(Node* &node, Account* account) {
    if (node == nullptr) {
        node = new Node();
        node->account_ = account;
        return true;
    }
    if (account->account_id() == node->account_->account_id()) {
        return false;
    }
    if (account->account_id() < node->account_->account_id()) {
        return Insert(node->left_, account);
    }
    if (account->account_id() > node->account_->account_id()) {
        return Insert(node->right_, account);
    }
    return false;
}

// Returns size of BSTree
int BSTree::Size() const {
    return Size(root_);
}

// Recursively returns size of BSTree
int BSTree::Size(Node* root) const {
    if (root == nullptr) {
        return 0;
    }
    return Size(root->left_) + Size(root->right_) + 1;
}

// Assignment Overload for Copy Constructor
BSTree &BSTree::operator=(const BSTree &tree) {
    if (this == &tree) {
        return *this;
    }
    DeleteNode(root_);
    CopyTree(tree.root_, root_);
    return *this;
}

// Recursively copies tree for Copy Constructor and Assignment Overload
void BSTree::CopyTree(Node *root, Node* &node) {
    if (root == nullptr) {
        node = nullptr;
        return;
    }
    node = new Node();
    node->account_ = new Account(*(root->account_));
    CopyTree(root->left_, node->left_);
    CopyTree(root->right_, node->right_);
}

// Output Overload for BSTree
ostream& operator<<(ostream &os, const BSTree &tree) {
    tree.PrintInOrder(os, tree.root_);
    return os;
}

// Recursively prints accounts in BSTree
void BSTree::PrintInOrder(ostream &os, Node *root) const {
    if (root == nullptr) {
        return;
    }
    PrintInOrder(os, root->left_);
    os << root->account_->account_id() << ", " << root->account_->first_name() << " " << root->account_->last_name() << endl;
    PrintInOrder(os, root->right_);
}

// Prints all accounts and their fund balances
void BSTree::PrintAll() {
    if (root_ == nullptr) {
        return;
    }
    PrintAll(root_->left_);
    cout << root_->account_->first_name() << " " << root_->account_->last_name() << " Account ID: " << root_->account_->account_id() << endl;
    for (int i = 0; i < 10; i++) {
        cout << "    " << root_->account_->fund_name(i) << ": " << root_->account_->funds_balance(i) << endl;
    }
    cout << endl;
    PrintAll(root_->right_);
}

// Recursively prints all accounts and their fund balances
void BSTree::PrintAll(Node *node) {
    if (node == nullptr) {
        return;
    }
    PrintAll(node->left_);
    cout << node->account_->first_name() << " " << node->account_->last_name() << " Account ID: " << node->account_->account_id() << endl;
    for (int i = 0; i < 10; i++) {
        cout << "    " << node->account_->fund_name(i) << ": " << node->account_->funds_balance(i) << endl;
    }
    cout << endl;
    PrintAll(node->right_);
}
