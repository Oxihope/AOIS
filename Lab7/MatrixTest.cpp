#include <gtest/gtest.h>
#include "Matrix.h"

TEST(LogicFunctionsTest, f2) {
    EXPECT_FALSE(f2(false, false));
    EXPECT_FALSE(f2(false, true));
    EXPECT_TRUE(f2(true, false));
    EXPECT_FALSE(f2(true, true));
}

TEST(LogicFunctionsTest, f7) {
    EXPECT_FALSE(f7(false, false));
    EXPECT_TRUE(f7(false, true));
    EXPECT_TRUE(f7(true, false));
    EXPECT_TRUE(f7(true, true));
}

TEST(LogicFunctionsTest, f8) {
    EXPECT_TRUE(f8(false, false));
    EXPECT_FALSE(f8(false, true));
    EXPECT_FALSE(f8(true, false));
    EXPECT_FALSE(f8(true, true));
}

TEST(LogicFunctionsTest, f13) {
    EXPECT_TRUE(f13(false, false));
    EXPECT_TRUE(f13(false, true));
    EXPECT_FALSE(f13(true, false));
    EXPECT_TRUE(f13(true, true));
}

TEST(ConversionTest, DecToBin3) {
    EXPECT_EQ(Dec_To_Bin_3(5), std::vector<int>({ 1, 0, 1 }));
    EXPECT_EQ(Dec_To_Bin_3(0), std::vector<int>({ 0, 0, 0 }));
}

TEST(ConversionTest, DecToBin16) {
    EXPECT_EQ(Dec_To_Bin_16(0), std::vector<int>(16, 0));
    EXPECT_EQ(Dec_To_Bin_16(65535), std::vector<int>(16, 1));
}

TEST(ConversionTest, PryamToDec) {
    EXPECT_EQ(Pryam_To_Dec({ 1, 0, 1 }), 5);
    EXPECT_EQ(Pryam_To_Dec({ 0, 0, 0 }), 0);
}

TEST(IndexTest, GetIndexVector) {
    EXPECT_EQ(get_index_vector(1, 4), std::vector<int>({ 1, 2, 3, 0 }));
    EXPECT_EQ(get_index_vector(0, 4), std::vector<int>({ 0, 1, 2, 3 }));
}

TEST(MatrixTest, WriteAndReadWord) {
    std::vector<std::vector<int>> matrix(4, std::vector<int>(4, 0));
    std::vector<int> word = { 1, 0, 1, 1 };
    write_word(matrix, 1, word);
    auto result = read_word(matrix, 1);
    EXPECT_EQ(result, word);
}

TEST(MatrixTest, WriteAndReadCol) {
    std::vector<std::vector<int>> matrix(4, std::vector<int>(4, 0));
    std::vector<int> col = { 1, 0, 1, 1 };
    write_col(matrix, 2, col);
    auto result = read_col(matrix, 2);
    EXPECT_EQ(result, col);
}

TEST(MatrixTest, CalculateWord) {
    std::vector<int> A = { 1, 0, 1 };
    std::vector<int> B = { 0, 1, 1 };
    std::vector<int> expected = { 1, 0, 0 }; // A & !B
    auto result = calculate_word(A, B, f2);
    EXPECT_EQ(result, expected);
}

TEST(MatrixTest, BinSum) {
    std::vector<int> A = { 1, 0, 1, 1 };
    std::vector<int> B = { 0, 1, 0, 1 };
    auto result = Bin_Sum(A, B); // 1011 + 0101 = 10000 (в 5 битах)
    EXPECT_EQ(result, std::vector<int>({ 1, 0, 0, 0, 0 }));
}

TEST(MatrixTest, FindWords) {
    std::vector<std::vector<int>> matrix(4, std::vector<int>(4, 0));
    matrix[0][0] = 1;
    matrix[1][0] = 0;
    matrix[2][0] = 1;
    auto result = find_words(matrix, 5); // bin(5) = 101
    EXPECT_EQ(result, std::vector<int>({ 0 }));
}

TEST(BinaryLogicTest, SumFieldsCorrect) {
    vector<vector<int>> Table(16, vector<int>(16, 0));

    // Создаем слово: V = 3 (011), A = 5 (0101), B = 6 (0110)
    vector<int> word = {
        0, 1, 1,     // V (dec=3)
        0, 1, 0, 1,  // A = 5
        0, 1, 1, 0,  // B = 6
        0, 0, 0, 0, 0  // S (заполнится функцией)
    };

    write_word(Table, 0, word);

    vector<int> modified = sum_fields(Table, 3);  // decV = 3 = 011
    vector<int> updated_word = read_word(Table, 0);

    vector<int> expected_sum = Bin_Sum({ 0, 1, 0, 1 }, { 0, 1, 1, 0 }); // A + B = 5 + 6 = 11 = 01011

    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(updated_word[11 + i], expected_sum[i]);
    }
    ASSERT_EQ(modified.size(), 1);
    EXPECT_EQ(modified[0], 0);
}

TEST(BinaryLogicTest, CompareTwoWordsEqual) {
    vector<int> a = Dec_To_Bin_16(42);
    vector<int> b = Dec_To_Bin_16(42);
    EXPECT_TRUE(compare_two_words(a, b));
}

TEST(BinaryLogicTest, CompareTwoWordsLess) {
    vector<int> a = Dec_To_Bin_16(10);
    vector<int> b = Dec_To_Bin_16(100);
    EXPECT_FALSE(compare_two_words(a, b));
}

TEST(BinaryLogicTest, CompareTwoWordsGreater) {
    vector<int> a = Dec_To_Bin_16(150);
    vector<int> b = Dec_To_Bin_16(100);
    EXPECT_TRUE(compare_two_words(a, b));
}

TEST(BinaryLogicTest, FindWordsInRangeWorks) {
    vector<vector<int>> Table(16, vector<int>(16, 0));

    // Запишем слово в позицию 2
    vector<int> word = Dec_To_Bin_16(1234);
    write_word(Table, 2, word);

    vector<int> found = find_words_in_range(Table, 1200, 1300);
    ASSERT_EQ(found.size(), 1);
    EXPECT_EQ(found[0], 2);

    // Проверим, что слово не находится вне диапазона
    found = find_words_in_range(Table, 100, 200);
    EXPECT_TRUE(found.empty());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}