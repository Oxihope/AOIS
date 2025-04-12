#include <gtest/gtest.h>
#include <sstream>
#include "BinCode.h"

TEST(BinCodeTEST, Dec_To_Pryam_TEST) {
    Bin a = Dec_To_Pryam(5);
    Bin b = Dec_To_Pryam(-5);
    EXPECT_EQ(a.znak, 0);
    EXPECT_EQ(a.int_part, vector<int>({ 1, 0, 1 }));
    EXPECT_EQ(b.znak, 1);
    EXPECT_EQ(b.int_part, vector<int>({ 1, 0, 1 }));
}

TEST(BinCodeTEST, Pryam_To_Dec_TEST) {
    Bin a;
    a.znak = 0;
    a.sostoyanie = 0;
    a.int_part = { 1,0,1 };
    a.double_part = { 1,0,0,1,0 };
    EXPECT_EQ(Pryam_To_Dec(a), 5.5625);
    a.znak = 1;
    EXPECT_EQ(Pryam_To_Dec(a), -5.5625);
}

TEST(BinCodeTEST, Pryam_To_Obratn_TEST) {
    Bin a = Dec_To_Pryam(-5);
    a = Pryam_To_Obratn(a);
    EXPECT_EQ(a.sostoyanie, 1);
    EXPECT_EQ(a.int_part, vector<int>({0,1,0}));
}

TEST(BinCodeTEST, Pryam_To_Dopoln_TEST) {
    Bin a = Dec_To_Pryam(-4);
    a = Pryam_To_Dopoln(a);
    EXPECT_EQ(a.sostoyanie, 2);
    EXPECT_EQ(a.int_part, vector<int>({ 1,0,0 }));
}

TEST(BinCodeTEST, Dopoln_To_Pryam_TEST) {
    Bin a = Dec_To_Pryam(-4);
    a = Pryam_To_Dopoln(a);
    a = Dopoln_To_Pryam(a);
    EXPECT_EQ(a.sostoyanie, 0);
    EXPECT_EQ(a.int_part, vector<int>({ 1,0,0 }));
}

TEST(BinCodeTEST, Bin_Sum_TEST) {
    Bin bin11 = Dec_To_Pryam(5);
    Bin bin12 = Dec_To_Pryam(10);
    Bin bin13 = Bin_Sum(Pryam_To_Dopoln(bin11), Pryam_To_Dopoln(bin12));
    EXPECT_EQ(Pryam_To_Dec(Dopoln_To_Pryam(bin13)),15);
    Bin bin21 = Dec_To_Pryam(10);
    Bin bin22 = Dec_To_Pryam(5);
    Bin bin23 = Bin_Sum(Pryam_To_Dopoln(bin21), Pryam_To_Dopoln(bin22));
    EXPECT_EQ(Pryam_To_Dec(Dopoln_To_Pryam(bin23)), 15);
    Bin bin31 = Dec_To_Pryam(10);
    Bin bin32 = Dec_To_Pryam(10);
    Bin bin33 = Bin_Sum(Pryam_To_Dopoln(bin31), Pryam_To_Dopoln(bin32));
    EXPECT_EQ(Pryam_To_Dec(Dopoln_To_Pryam(bin33)), 20);
}

TEST(BinCodeTEST, Delete_Zero_TEST) {
    Bin a = Dec_To_Pryam(5);
    a.int_part.insert(a.int_part.begin(), 0);
    a.int_part.insert(a.int_part.begin(), 0);
    a.int_part.insert(a.int_part.begin(), 0);
    EXPECT_EQ(a.int_part, vector<int>({0,0,0,1,0,1}));
    a = Delete_Zero(a);
    EXPECT_EQ(a.int_part, vector<int>({ 1,0,1 }));
}

TEST(BinCodeTEST, Bin_Multiply_TEST) {
    Bin a = Dec_To_Pryam(2);
    Bin b = Dec_To_Pryam(3);
    Bin c = Bin_Multiply(a, b);
    EXPECT_EQ(Pryam_To_Dec(c), 6);
    Bin d = Dec_To_Pryam(6);
    Bin e = Dec_To_Pryam(3);
    Bin f = Bin_Multiply(d, e);
    EXPECT_EQ(Pryam_To_Dec(f), 18);
}

TEST(BinCodeTEST, Devide_Bin_TEST) {
    Bin a = Dec_To_Pryam(5);
    Bin b = Dec_To_Pryam(-3);
    Bin c = Devide_Bin(a, b);
    EXPECT_EQ(Pryam_To_Dec(c), -1.65625);
}

TEST(BinCodeTEST, Cout_Bin_TEST) {
    Bin a = Dec_To_Pryam(5);

    ostringstream output;
    streambuf* oldCoutBuffer = cout.rdbuf(output.rdbuf());
    Cout_Bin(a);
    cout.rdbuf(oldCoutBuffer);
    string expectedOutput = "Прямой код: 0 101.";
    EXPECT_EQ(output.str(), expectedOutput);

    Bin b = Dec_To_Pryam(5);
    b = Pryam_To_Obratn(b);

    output.str("");
    output.clear();

    oldCoutBuffer = cout.rdbuf(output.rdbuf());
    Cout_Bin(b);
    cout.rdbuf(oldCoutBuffer);
    expectedOutput = "Обратный код: 0 101.";
    EXPECT_EQ(output.str(), expectedOutput);

    Bin c = Dec_To_Pryam(5);
    c = Pryam_To_Dopoln(c);

    output.str("");
    output.clear();

    oldCoutBuffer = cout.rdbuf(output.rdbuf());
    Cout_Bin(c);
    cout.rdbuf(oldCoutBuffer);
    expectedOutput = "Дополнительный код: 0 101.";
    EXPECT_EQ(output.str(), expectedOutput);

    Bin d;
    d.sostoyanie = 0;
    d.znak = 0;
    d.int_part = { 1,0,1 };
    d.double_part = {1,0,1,1,0};

    output.str("");
    output.clear();

    oldCoutBuffer = cout.rdbuf(output.rdbuf());
    Cout_Bin(d);
    cout.rdbuf(oldCoutBuffer);
    expectedOutput = "Прямой код: 0 101.10110";
    EXPECT_EQ(output.str(), expectedOutput);
}

TEST(BinCodeTEST, Double_To_Plav_TEST) {
    Bin a = Double_To_Plav(15.75);
    EXPECT_EQ(a.sostoyanie, 0);
    EXPECT_EQ(a.znak, 0);
    EXPECT_EQ(a.int_part, vector<int>({1,0,0,0,0,0,1,0}));
    EXPECT_EQ(a.double_part, vector<int>({ 1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }));
    Bin b = Double_To_Plav(-0.75);
    EXPECT_EQ(b.sostoyanie, 0);
    EXPECT_EQ(b.znak, 1);
    EXPECT_EQ(b.int_part, vector<int>({ 0,1,1,1,1,1,1,0 }));
    EXPECT_EQ(b.double_part, vector<int>({ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }));
}

TEST(BinCodeTEST, Plav_To_Pryam_TEST) {
    Bin a = Double_To_Plav(15.75);
    a = Plav_To_Pryam(a);
    EXPECT_EQ(a.sostoyanie, 0);
    EXPECT_EQ(a.znak, 0);
    EXPECT_EQ(a.int_part, vector<int>({ 1,1,1,1 }));
    EXPECT_EQ(a.double_part, vector<int>({ 1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }));
}


TEST(BinCodeTEST, Plav_Bin_Sum_TEST) {
    Bin a = Double_To_Plav(15.125);
    Bin b = Double_To_Plav(-0.125);
    Bin c = Plav_Bin_Sum(a, b);
    EXPECT_EQ(Pryam_To_Dec(Plav_To_Pryam(c)), 15);
    a = Double_To_Plav(15);
    b = Double_To_Plav(-7);
    c = Plav_Bin_Sum(a, b);
    EXPECT_EQ(Pryam_To_Dec(Plav_To_Pryam(c)), 8);
    a = Double_To_Plav(-7);
    b = Double_To_Plav(15);
    c = Plav_Bin_Sum(a, b);
    EXPECT_EQ(Pryam_To_Dec(Plav_To_Pryam(c)), 8);
    a = Double_To_Plav(15);
    b = Double_To_Plav(-14);
    c = Plav_Bin_Sum(a, b);
    EXPECT_EQ(Pryam_To_Dec(Plav_To_Pryam(c)), 1);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}