#include <gtest/gtest.h>
#include "HashTable.h"

TEST(HashTableTest, InsertAndSearch) {
    HashTable ht(8);
    ht.insert("Pushkin", 1799);
    ht.insert("Tolstoy", 1828);
    int data;

    EXPECT_TRUE(ht.search("Pushkin", data));
    EXPECT_EQ(data, 1799);

    EXPECT_TRUE(ht.search("Tolstoy", data));
    EXPECT_EQ(data, 1828);

    EXPECT_FALSE(ht.search("Dostoevsky", data));  // �� ���������
}

// ���� ���������� ������������� �����
TEST(HashTableTest, UpdateData) {
    HashTable ht(8);
    ht.insert("Pushkin", 1799);
    ht.update("Pushkin", 1800);
    int data;

    EXPECT_TRUE(ht.search("Pushkin", data));
    EXPECT_EQ(data, 1800);
}

// ���� �������� �����
TEST(HashTableTest, RemoveKey) {
    HashTable ht(8);
    ht.insert("Pushkin", 1799);
    int data;

    EXPECT_TRUE(ht.search("Pushkin", data));
    ht.remove("Pushkin");
    EXPECT_FALSE(ht.search("Pushkin", data));
}

// ���� ������ ������� � �����
TEST(HashTableTest, PrintTable) {
    HashTable ht(2);
    ht.insert("A", 1);
    ht.insert("B", 2);

    std::ostringstream oss;
    std::streambuf* old_cout = std::cout.rdbuf(oss.rdbuf());  // �������������� cout

    ht.print();

    std::cout.rdbuf(old_cout);  // ��������������� cout

    std::string output = oss.str();
    EXPECT_NE(output.find("USED"), std::string::npos);
    EXPECT_NE(output.find("A:1"), std::string::npos);
    EXPECT_NE(output.find("B:2"), std::string::npos);
}

// ���� rotateRight
TEST(TreeNodeRotationTest, RotateRight) {
    // ������� ���� �������
    auto root = make_shared<TreeNode>("B", 2);
    root->left = make_shared<TreeNode>("A", 1);
    root->right = nullptr;

    // ��������� ������
    updateHeight(root->left);
    updateHeight(root);

    // ������� ������
    auto newRoot = rotateRight(root);

    // ���������, ��� ����� ������ - "A"
    EXPECT_EQ(newRoot->key, "A");
    // "B" ���� ������ ����� "A"
    ASSERT_TRUE(newRoot->right);
    EXPECT_EQ(newRoot->right->key, "B");
    // ����� ��� "B" ���� nullptr
    EXPECT_EQ(newRoot->right->left, nullptr);
}

// ���� rotateLeft
TEST(TreeNodeRotationTest, RotateLeft) {
    auto root = make_shared<TreeNode>("A", 1);
    root->right = make_shared<TreeNode>("B", 2);
    root->left = nullptr;

    updateHeight(root->right);
    updateHeight(root);

    auto newRoot = rotateLeft(root);

    EXPECT_EQ(newRoot->key, "B");
    ASSERT_TRUE(newRoot->left);
    EXPECT_EQ(newRoot->left->key, "A");
    EXPECT_EQ(newRoot->left->right, nullptr);
}

// ���� findMin
TEST(TreeNodeTest, FindMin) {
    auto root = make_shared<TreeNode>("C", 3);
    root->left = make_shared<TreeNode>("B", 2);
    root->left->left = make_shared<TreeNode>("A", 1);
    root->right = nullptr;

    auto minNode = findMin(root);
    ASSERT_NE(minNode, nullptr);
    EXPECT_EQ(minNode->key, "A");
    EXPECT_EQ(minNode->data, 1);
}

TEST(BalanceTest, LL_Balance) {
    shared_ptr<TreeNode> root = nullptr;
    root = insert_node(root, "C", 3);
    root = insert_node(root, "B", 2);
    root = insert_node(root, "A", 1);  // ������� LL-�������

    EXPECT_EQ(root->key, "B");
    EXPECT_EQ(root->left->key, "A");
    EXPECT_EQ(root->right->key, "C");
}

TEST(BalanceTest, RR_Balance) {
    shared_ptr<TreeNode> root = nullptr;
    root = insert_node(root, "A", 1);
    root = insert_node(root, "B", 2);
    root = insert_node(root, "C", 3);  // ������� RR-�������

    EXPECT_EQ(root->key, "B");
    EXPECT_EQ(root->left->key, "A");
    EXPECT_EQ(root->right->key, "C");
}

TEST(BalanceTest, LR_Balance) {
    shared_ptr<TreeNode> root = nullptr;
    root = insert_node(root, "C", 3);
    root = insert_node(root, "A", 1);
    root = insert_node(root, "B", 2);  // ������� LR-�������

    EXPECT_EQ(root->key, "B");
    EXPECT_EQ(root->left->key, "A");
    EXPECT_EQ(root->right->key, "C");
}

TEST(BalanceTest, RL_Balance) {
    shared_ptr<TreeNode> root = nullptr;
    root = insert_node(root, "A", 1);
    root = insert_node(root, "C", 3);
    root = insert_node(root, "B", 2);  // ������� RL-�������

    EXPECT_EQ(root->key, "B");
    EXPECT_EQ(root->left->key, "A");
    EXPECT_EQ(root->right->key, "C");
}

TEST(RemoveNodeTest, RemoveLeaf) {
    shared_ptr<TreeNode> root = nullptr;
    root = insert_node(root, "B", 2);
    root = insert_node(root, "A", 1);
    root = insert_node(root, "C", 3);

    root = remove_node(root, "C");  // ������� ����
    EXPECT_EQ(find(root, "C"), nullptr);
    EXPECT_NE(find(root, "A"), nullptr);
}

TEST(RemoveNodeTest, RemoveNodeWithOneChild) {
    shared_ptr<TreeNode> root = nullptr;
    root = insert_node(root, "A", 1);
    root = insert_node(root, "B", 2);
    root = insert_node(root, "C", 3);

    root = remove_node(root, "B");  // B ����� ������� ������� C
    EXPECT_EQ(find(root, "B"), nullptr);
    EXPECT_NE(find(root, "C"), nullptr);
}

TEST(RemoveNodeTest, RemoveNodeWithTwoChildren) {
    shared_ptr<TreeNode> root = nullptr;
    root = insert_node(root, "B", 2);
    root = insert_node(root, "A", 1);
    root = insert_node(root, "C", 3);

    root = remove_node(root, "B");  // B ����� ���� �����
    EXPECT_EQ(find(root, "B"), nullptr);
    EXPECT_NE(find(root, "A"), nullptr);
    EXPECT_NE(find(root, "C"), nullptr);
}

TEST(HashTableTest, PrintOutput) {
    HashTable ht(4);
    ht.insert("Alpha", 1);
    ht.insert("Beta", 2);

    // �������������� cout �� ��������� stringstream
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    ht.print();

    std::cout.rdbuf(oldCout);  // ���������� cout �������
    std::string output = buffer.str();

    // ���������, ��� � ������ ���� ����������� �����
    EXPECT_NE(output.find("Alpha"), std::string::npos);
    EXPECT_NE(output.find("Beta"), std::string::npos);
    EXPECT_NE(output.find("USED"), std::string::npos);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}