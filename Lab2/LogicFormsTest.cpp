#include <gtest/gtest.h>
#include "LogicForms.h"

TEST(LogicFormsTest, Prior_TEST) {
    EXPECT_EQ(Prior('a'), 0);
    EXPECT_EQ(Prior('('), 1);
    EXPECT_EQ(Prior('&'), 2);
    EXPECT_EQ(Prior('|'), 2);
    EXPECT_EQ(Prior('-'), 2);
    EXPECT_EQ(Prior('~'), 2);
    EXPECT_EQ(Prior(')'), 3);
    EXPECT_EQ(Prior('!'), 4);
}

TEST(LogicFormsTest, Prior_Action_TEST) {
    vector<char> charVector = {'(','(','a','|','b',')','&','(','!','c',')',')'};
    stack* begin = new stack;
    stack* root = begin;
    for (int i = 0; i < charVector.size(); i++) {
        begin = Prior_Action(begin, charVector[i]);
    }
    EXPECT_EQ(root->info, '&');
    EXPECT_EQ(root->prev, nullptr);
    EXPECT_EQ(root->left->info, '|');
    EXPECT_EQ(root->left->prev, root);
    EXPECT_EQ(root->left->left->info, 'a');
    EXPECT_EQ(root->left->left->prev, root->left);
    EXPECT_EQ(root->left->left->left, nullptr);
    EXPECT_EQ(root->left->left->right, nullptr);
    EXPECT_EQ(root->left->right->info, 'b');
    EXPECT_EQ(root->left->right->prev, root->left);
    EXPECT_EQ(root->left->right->left, nullptr);
    EXPECT_EQ(root->left->right->right, nullptr);
    EXPECT_EQ(root->right->info, '!');
    EXPECT_EQ(root->right->prev, root);
    EXPECT_EQ(root->right->left->info, 'c');
    EXPECT_EQ(root->right->left->prev, root->right);
    EXPECT_EQ(root->right->left->left, nullptr);
    EXPECT_EQ(root->right->left->right, nullptr);
    EXPECT_EQ(root->right->right, nullptr);
}

TEST(LogicFormsTest, Ñonjunction_TEST) {
    EXPECT_EQ(Ñonjunction(0,0), 0);
    EXPECT_EQ(Ñonjunction(0,1), 0);
    EXPECT_EQ(Ñonjunction(1,0), 0);
    EXPECT_EQ(Ñonjunction(1,1), 1);
}

TEST(LogicFormsTest, Disjunction_TEST) {
    EXPECT_EQ(Disjunction(0, 0), 0);
    EXPECT_EQ(Disjunction(0, 1), 1);
    EXPECT_EQ(Disjunction(1, 0), 1);
    EXPECT_EQ(Disjunction(1, 1), 1);
}

TEST(LogicFormsTest, Implication_TEST) {
    EXPECT_EQ(Implication(0, 0), 1);
    EXPECT_EQ(Implication(0, 1), 1);
    EXPECT_EQ(Implication(1, 0), 0);
    EXPECT_EQ(Implication(1, 1), 1);
}

TEST(LogicFormsTest, Equivalence_TEST) {
    EXPECT_EQ(Equivalence(0, 0), 1);
    EXPECT_EQ(Equivalence(0, 1), 0);
    EXPECT_EQ(Equivalence(1, 0), 0);
    EXPECT_EQ(Equivalence(1, 1), 1);
}

TEST(LogicFormsTest, Negation_TEST) {
    EXPECT_EQ(Negation(0), 1);
    EXPECT_EQ(Negation(1), 0);
}

TEST(LogicFormsTest, CollectNodes_TEST) {
    vector<char> charVector = { '(','(','a','|','b',')','&','(','!','c',')',')' };
    stack* begin = new stack;
    stack* root = begin;
    for (int i = 0; i < charVector.size(); i++) {
        begin = Prior_Action(begin, charVector[i]);
    }
    vector<stack*> nodes;
    CollectNodes(root, nodes);
    EXPECT_EQ(nodes[0], root->left->left);
    EXPECT_EQ(nodes[1], root->left->right);
    EXPECT_EQ(nodes[2], root->left);
    EXPECT_EQ(nodes[3], root->right->left);
    EXPECT_EQ(nodes[4], root->right);
    EXPECT_EQ(nodes[5], root);
}

TEST(LogicFormsTest, Result_TEST) {
    vector<char> charVector = { '(','(','(','a','-','b',')','|','c',')','~','(','d','&','(','!','e',')',')',')'};
    stack* begin = new stack;
    stack* root = begin;
    for (int i = 0; i < charVector.size(); i++) {
        begin = Prior_Action(begin, charVector[i]);
    }
    vector<bool> stepresults;
    bool result = Result(root, 0, 1, 0, 1, 1, stepresults);
    EXPECT_EQ(result, 0);
}

TEST(LogicFormsTest, BuildTruthTable_TEST) {
    vector<char> charVector = { '(','(','(','a','-','b',')','|','c',')','~','(','d','&','(','!','e',')',')',')' };
    stack* begin = new stack;
    stack* root = begin;
    for (int i = 0; i < charVector.size(); i++) {
        begin = Prior_Action(begin, charVector[i]);
    }
    vector<char> Variables = { 'a', 'b', 'c', 'd', 'e' };
    vector<vector<bool>> TruthTable = BuildTruthTable(root, Variables);
    vector<char> charVector1 = { '(','a','|','b',')'};
    stack* begin1 = new stack;
    stack* root1 = begin1;
    for (int i = 0; i < charVector1.size(); i++) {
        begin1 = Prior_Action(begin1, charVector1[i]);
    }
    vector<char> Variables1 = { 'a', 'b' };
    vector<vector<bool>> TruthTable1 = BuildTruthTable(root1, Variables1);
    EXPECT_EQ(TruthTable1[0][0], 0); EXPECT_EQ(TruthTable1[0][1], 0); EXPECT_EQ(TruthTable1[0][2], 0);
    EXPECT_EQ(TruthTable1[1][0], 0); EXPECT_EQ(TruthTable1[1][1], 1); EXPECT_EQ(TruthTable1[1][2], 1);
    EXPECT_EQ(TruthTable1[2][0], 1); EXPECT_EQ(TruthTable1[2][1], 0); EXPECT_EQ(TruthTable1[2][2], 1);
    EXPECT_EQ(TruthTable1[3][0], 1); EXPECT_EQ(TruthTable1[3][1], 1); EXPECT_EQ(TruthTable1[3][2], 1);
}

TEST(LogicFormTest, SDNF_TEST) {
    vector<char> charVector = { '(','a','~','b',')' };
    stack* begin = new stack;
    stack* root = begin;
    for (int i = 0; i < charVector.size(); i++) {
        begin = Prior_Action(begin, charVector[i]);
    }
    vector<char> Variables = { 'a', 'b' };
    vector<vector<bool>> TruthTable = BuildTruthTable(root, Variables);
    vector<char> sdnf = SDNF(TruthTable, Variables);
    string str(sdnf.begin(), sdnf.end());
    EXPECT_EQ(str, "((!a)&(!b)) | (a&b)");
}

TEST(LogicFormTest, SKNF_TEST) {
    vector<char> charVector = { '(','a','~','b',')' };
    stack* begin = new stack;
    stack* root = begin;
    for (int i = 0; i < charVector.size(); i++) {
        begin = Prior_Action(begin, charVector[i]);
    }
    vector<char> Variables = { 'a', 'b' };
    vector<vector<bool>> TruthTable = BuildTruthTable(root, Variables);
    vector<char> sknf = SKNF(TruthTable, Variables);
    string str(sknf.begin(), sknf.end());
    EXPECT_EQ(str, "(a|(!b)) & ((!a)|b)");
}

TEST(LogicFormTest, SDNF_number_form_TEST) {
    vector<char> charVector = { '(','a','~','b',')' };
    stack* begin = new stack;
    stack* root = begin;
    for (int i = 0; i < charVector.size(); i++) {
        begin = Prior_Action(begin, charVector[i]);
    }
    vector<char> Variables = { 'a', 'b' };
    vector<vector<bool>> TruthTable = BuildTruthTable(root, Variables);
    vector<char> sdnf = SDNF_number_form(TruthTable);
    string str(sdnf.begin(), sdnf.end());
    EXPECT_EQ(str, "(0,3) |");
}

TEST(LogicFormTest, SKNF_number_form_TEST) {
    vector<char> charVector = { '(','a','~','b',')' };
    stack* begin = new stack;
    stack* root = begin;
    for (int i = 0; i < charVector.size(); i++) {
        begin = Prior_Action(begin, charVector[i]);
    }
    vector<char> Variables = { 'a', 'b' };
    vector<vector<bool>> TruthTable = BuildTruthTable(root, Variables);
    vector<char> sknf = SKNF_number_form(TruthTable);
    string str(sknf.begin(), sknf.end());
    EXPECT_EQ(str, "(1,2) &");
}

TEST(LogicFormTest, Index_form_TEST) {
    vector<char> charVector = { '(','(','a','|','b',')','&','(','!','c',')',')' };
    stack* begin = new stack;
    stack* root = begin;
    for (int i = 0; i < charVector.size(); i++) {
        begin = Prior_Action(begin, charVector[i]);
    }
    vector<char> Variables = { 'a', 'b', 'c' };
    vector<vector<bool>> TruthTable = BuildTruthTable(root, Variables);
    vector<int> index_form_vec;
    int index_form = Index_form(TruthTable, index_form_vec);
    EXPECT_EQ(index_form, 42);
    EXPECT_EQ(index_form_vec[0], 0);
    EXPECT_EQ(index_form_vec[1], 0);
    EXPECT_EQ(index_form_vec[2], 1);
    EXPECT_EQ(index_form_vec[3], 0);
    EXPECT_EQ(index_form_vec[4], 1);
    EXPECT_EQ(index_form_vec[5], 0);
    EXPECT_EQ(index_form_vec[6], 1);
    EXPECT_EQ(index_form_vec[7], 0);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}