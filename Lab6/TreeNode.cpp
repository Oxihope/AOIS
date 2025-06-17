#include "TreeNode.h"

int height(shared_ptr<TreeNode> node) {
    return node ? node->height : 0;
}

int balanceFactor(shared_ptr<TreeNode> node) {
    return height(node->left) - height(node->right);
}

void updateHeight(shared_ptr<TreeNode> node) {
    node->height = 1 + max(height(node->left), height(node->right));
}

shared_ptr<TreeNode> rotateRight(shared_ptr<TreeNode> y) {
    shared_ptr<TreeNode> x = y->left;
    y->left = x->right;
    x->right = y;
    updateHeight(y);
    updateHeight(x);
    return x;
}

shared_ptr<TreeNode> rotateLeft(shared_ptr<TreeNode> x) {
    shared_ptr<TreeNode> y = x->right;
    x->right = y->left;
    y->left = x;
    updateHeight(x);
    updateHeight(y);
    return y;
}

shared_ptr<TreeNode> balance(shared_ptr<TreeNode> node) {
    updateHeight(node);
    if (balanceFactor(node) > 1) {
        if (balanceFactor(node->left) < 0)
            node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balanceFactor(node) < -1) {
        if (balanceFactor(node->right) > 0)
            node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

shared_ptr<TreeNode> insert_node(shared_ptr<TreeNode> root, const string& key, int data) {
    if (!root) return make_shared<TreeNode>(key, data);
    if (key < root->key) root->left = insert_node(root->left, key, data);
    else if (key > root->key) root->right = insert_node(root->right, key, data);
    else root->data = data;
    return balance(root);
}

shared_ptr<TreeNode> find(shared_ptr<TreeNode> root, const string& key) {
    if (!root) return nullptr;
    if (key < root->key) return find(root->left, key);
    if (key > root->key) return find(root->right, key);
    return root;
}

shared_ptr<TreeNode> findMin(shared_ptr<TreeNode> node) {
    while (node->left) node = node->left;
    return node;
}

shared_ptr<TreeNode> remove_node(shared_ptr<TreeNode> root, const string& key) {
    if (!root) return nullptr;
    if (key < root->key) root->left = remove_node(root->left, key);
    else if (key > root->key) root->right = remove_node(root->right, key);
    else {
        if (!root->left) return root->right;
        if (!root->right) return root->left;
        shared_ptr<TreeNode> minRight = findMin(root->right);
        root->key = minRight->key;
        root->data = minRight->data;
        root->right = remove_node(root->right, minRight->key);
    }
    return balance(root);
}

void printTree(const shared_ptr<TreeNode>& node, const string& prefix, bool isLeft) {
    if (node) {
        cout << prefix;
        cout << (isLeft ? "|l---" : "|r___");
        cout << node->key << ":" << node->data << endl;
        printTree(node->left, prefix + (isLeft ? "|   " : "    "), true);
        printTree(node->right, prefix + (isLeft ? "|   " : "    "), false);
    }
}