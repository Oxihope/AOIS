#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

bool f2(bool x1, bool x2);

bool f7(bool x1, bool x2);

bool f8(bool x1, bool x2);

bool f13(bool x1, bool x2);

vector<int> get_index_vector(int index, int table_size);

void write_word(vector<vector<int>>& Table, int index, vector<int> new_word);

void write_col(vector<vector<int>>& Table, int index, vector<int> new_col);

vector<int> read_word(vector<vector<int>>& Table, int index);

vector<int> read_col(vector<vector<int>>& Table, int index);

vector<int> calculate_word(vector<int> word1, vector<int> word2, bool(*f)(bool, bool));

vector<int> Dec_To_Bin_3(int dec);

vector<int> Dec_To_Bin_16(int dec);

int Pryam_To_Dec(vector<int> bin);

vector<int> find_words(vector<vector<int>>& Table, int decV);

vector<int> Bin_Sum(vector<int> A, vector<int> B);

vector<int> sum_fields(vector<vector<int>>& Table, int decV);

bool compare_two_words(vector<int> a, vector<int> s);

vector<int> find_words_in_range(vector<vector<int>>& Table, int decDown, int decUp);

