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
    vector<char> charVector = { '(','(','a','|','b',')','&','(','!','c',')',')' };
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
    EXPECT_EQ(Conjunction(0, 0), 0);
    EXPECT_EQ(Conjunction(0, 1), 0);
    EXPECT_EQ(Conjunction(1, 0), 0);
    EXPECT_EQ(Conjunction(1, 1), 1);
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
    vector<char> charVector = { '(','(','(','a','-','b',')','|','c',')','~','(','d','&','(','!','e',')',')',')' };
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
    vector<char> charVector1 = { '(','a','|','b',')' };
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

TEST(LogicFormsTest, SDNF_TEST) {
    vector<char> charVector = { '(','a','~','b',')' };
    stack* begin = new stack;
    stack* root = begin;
    for (int i = 0; i < charVector.size(); i++) {
        begin = Prior_Action(begin, charVector[i]);
    }
    vector<char> Variables = { 'a', 'b' };
    vector<vector<bool>> TruthTable = BuildTruthTable(root, Variables);
    vector<form> sdnf = SDNF(TruthTable, Variables);
    string printsdnf = print_sdnf(sdnf);
    EXPECT_EQ(printsdnf, "((!a)&(!b)) | (a&b)");
}

TEST(LogicFormsTest, SKNF_TEST) {
    vector<char> charVector = { '(','a','~','b',')' };
    stack* begin = new stack;
    stack* root = begin;
    for (int i = 0; i < charVector.size(); i++) {
        begin = Prior_Action(begin, charVector[i]);
    }
    vector<char> Variables = { 'a', 'b' };
    vector<vector<bool>> TruthTable = BuildTruthTable(root, Variables);
    vector<form> sknf = SKNF(TruthTable, Variables);
    string printsknf = print_sknf(sknf);
    EXPECT_EQ(printsknf, "(a|(!b)) & ((!a)|b)");
}

TEST(LogicFormsTest, find_diff_var_TEST) {
    form form1;
    form form2;
    form form3;
    vector<char> var;
    var.push_back('a');
    var.push_back('b');
    var.push_back('c');
    vector<bool> val1;
    val1.push_back(1); val1.push_back(1); val1.push_back(1);
    vector<bool> val2;
    val2.push_back(1); val2.push_back(1); val2.push_back(0);
    form1.variables = var;
    form2.variables = var;
    form3.variables = var;
    form1.values = val1;
    form2.values = val1;
    form3.values = val2;
    int a = find_diff_var(form1, form2);
    int b = find_diff_var(form1, form3);
    EXPECT_EQ(a, -1);
    EXPECT_EQ(b, 2);
}

TEST(LogicFormsTest, merging_sdnf_TEST) {
    vector<char> charVector = { '(','(','a','-','b',')','&','c',')'};
    stack* begin = new stack;
    stack* root = begin;
    for (int i = 0; i < charVector.size(); i++) {
        begin = Prior_Action(begin, charVector[i]);
    }
    vector<char> Variables = { 'a', 'b', 'c'};
    vector<vector<bool>> TruthTable = BuildTruthTable(root, Variables);
    vector<form> sdnf = SDNF(TruthTable, Variables);
    vector<form> current_sdnf = merging_sdnf(sdnf); 
    string printsdnf = print_sdnf(current_sdnf);
    EXPECT_EQ(printsdnf, "((!a)&c) | (b&c)");
}

TEST(LogicFormsTest, merging_sknf_TEST) {
    vector<char> charVector = { '(','(','a','-','b',')','&','c',')' };
    stack* begin = new stack;
    stack* root = begin;
    for (int i = 0; i < charVector.size(); i++) {
        begin = Prior_Action(begin, charVector[i]);
    }
    vector<char> Variables = { 'a', 'b', 'c' };
    vector<vector<bool>> TruthTable = BuildTruthTable(root, Variables);
    vector<form> sknf = SKNF(TruthTable, Variables);
    vector<form> current_sknf = merging_sknf(sknf);
    string printsknf = print_sknf(current_sknf);
    EXPECT_EQ(printsknf, "(c) & ((!a)|b)");
}

TEST(LogicFormsTest, table_merging_sdnf_TEST) {
    vector<char> charVector = { '(','(','a','-','b',')','&','c',')' };
    stack* begin = new stack;
    stack* root = begin;
    for (int i = 0; i < charVector.size(); i++) {
        begin = Prior_Action(begin, charVector[i]);
    }
    vector<char> Variables = { 'a', 'b', 'c' };
    vector<vector<bool>> TruthTable = BuildTruthTable(root, Variables);
    vector<form> sdnf = SDNF(TruthTable, Variables);
    vector<vector<form>> table_sdnf = table_merging(sdnf);
    int col_width = Variables.size() * 5 + 3;
    string table = print_table_merging(table_sdnf, col_width, 1);
    EXPECT_EQ(table, " |((!a)&(!b)&c)|((!a)&b&c)|(a&b&c)|\n------------------------------------------------------------------------\n((!a)&c)|X|X| |\n------------------------------------------------------------------------\n(b&c)| |X|X|\n------------------------------------------------------------------------\n");
}

TEST(LogicFormsTest, table_merging_sknf_TEST) {
    vector<char> charVector = { '(','(','a','-','b',')','&','c',')' };
    stack* begin = new stack;
    stack* root = begin;
    for (int i = 0; i < charVector.size(); i++) {
        begin = Prior_Action(begin, charVector[i]);
    }
    vector<char> Variables = { 'a', 'b', 'c' };
    vector<vector<bool>> TruthTable = BuildTruthTable(root, Variables);
    vector<form> sknf = SKNF(TruthTable, Variables);
    vector<vector<form>> table_sknf = table_merging(sknf);
    int col_width = Variables.size() * 5 + 3;
    string table = print_table_merging(table_sknf, col_width, 0);
    EXPECT_EQ(table, " |((!a)|(!b)|(!c))|((!a)|b|(!c))|(a|(!b)|(!c))|(a|(!b)|c)|(a|b|(!c))|\n------------------------------------------------------------------------------------------------------------\n((!c))|X|X|X| |X|\n------------------------------------------------------------------------------------------------------------\n(a|(!b))| | |X|X| |\n------------------------------------------------------------------------------------------------------------\n");
}

TEST(LogicFormsTest, minimize_KMap_TEST) {
    vector<char> charVector = { '(','(','a','-','b',')','&','c',')' };
    stack* begin = new stack;
    stack* root = begin;
    for (int i = 0; i < charVector.size(); i++) {
        begin = Prior_Action(begin, charVector[i]);
    }
    vector<char> Variables = { 'a', 'b', 'c' };
    vector<vector<bool>> TruthTable = BuildTruthTable(root, Variables);
    vector<vector<bool>> kmap = BuildKMapFromTruthTable(TruthTable, Variables);
    vector<form> minimized_sknf = minimize_KMap(kmap, Variables, 0);
    string table = printKMap(kmap, 1, 2);
    EXPECT_EQ(table, " |00|01|11|10|\n ----+----+----+----+----+\n 0 |1 |0 |0 |1 |\n ----+----+----+----+----+\n 1 |1 |1 |0 |1 |\n ----+----+----+----+----+\n ");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}