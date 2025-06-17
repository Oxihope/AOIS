#include "HashTable.h"

int HashTable::hash(const string& key) {
    int h = 0;
    for (char c : key) h = (h * 31 + c) % size;
    return h;
}

void HashTable::insert(const string& key, int data) {
    int idx = hash(key);
    auto& cell = table[idx];
    cell.used = true;
    if (cell.chain) cell.collision = true;
    cell.chain = insert_node(cell.chain, key, data);
}

bool HashTable::search(const string& key, int& outData) {
    int idx = hash(key);
    auto& cell = table[idx];
    shared_ptr<TreeNode> found = find(cell.chain, key);
    if (found) {
        outData = found->data;
        return true;
    }
    return false;
}

void HashTable::remove(const string& key) {
    int idx = hash(key);
    auto& cell = table[idx];
    cell.chain = remove_node(cell.chain, key);
    if (!cell.chain) cell.used = false;
}

void HashTable::update(const string& key, int newData) {
    int idx = hash(key);
    auto& cell = table[idx];
    cell.chain = insert_node(cell.chain, key, newData);
}

void HashTable::print() {
    for (int i = 0; i < size; ++i) {
        cout << "[" << i << "] " << (table[i].used ? "USED" : "EMPTY") << (table[i].collision ? " (COLLISION)" : "") << endl;
        if (table[i].chain)
            printTree(table[i].chain);
        else
            cout << "r___ <empty>" << endl;
        cout << endl;
    }
}
