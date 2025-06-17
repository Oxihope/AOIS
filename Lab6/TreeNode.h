#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <iomanip>
using namespace std;

struct TreeNode {
    string key;
    int data;
    shared_ptr<TreeNode> left, right;
    int height;

    TreeNode(const string& k, int d) : key(k), data(d), height(1) {}
};

int height(shared_ptr<TreeNode> node);

int balanceFactor(shared_ptr<TreeNode> node);

void updateHeight(shared_ptr<TreeNode> node);

shared_ptr<TreeNode> rotateRight(shared_ptr<TreeNode> y);

shared_ptr<TreeNode> rotateLeft(shared_ptr<TreeNode> x);

shared_ptr<TreeNode> balance(shared_ptr<TreeNode> node);

shared_ptr<TreeNode> insert_node(shared_ptr<TreeNode> root, const string& key, int data);

shared_ptr<TreeNode> find(shared_ptr<TreeNode> root, const string& key);

shared_ptr<TreeNode> findMin(shared_ptr<TreeNode> node);

shared_ptr<TreeNode> remove_node(shared_ptr<TreeNode> root, const string& key);

void printTree(const shared_ptr<TreeNode>& node, const string& prefix = "", bool isLeft = true);