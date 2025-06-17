#include "TreeNode.h"

struct HashCell {
    bool collision = false;
    bool used = false;
    bool terminal = false;
    bool linked = false;
    bool deleted = false;
    shared_ptr<TreeNode> chain;
};

class HashTable {
    vector<HashCell> table;
    int size;

    int hash(const string& key);

public:
    HashTable(int s) : size(s), table(s) {}

    void insert(const string& key, int data);

    bool search(const string& key, int& outData);

    void remove(const string& key);

    void update(const string& key, int newData);

    void print();
};
