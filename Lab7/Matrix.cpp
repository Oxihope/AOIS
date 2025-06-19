#include "Matrix.h"

bool f2(bool x1, bool x2) {
    return (x1 && (!x2));
}

bool f7(bool x1, bool x2) {
    return (x1 || x2);
}

bool f8(bool x1, bool x2) {
    return (!(x1 || x2));
}

bool f13(bool x1, bool x2) {
    return ((!x1) || x2);
}

vector<int> get_index_vector(int index, int table_size) {
    vector<int> index_vector(table_size, 0);
    for (int i = 0; i < index_vector.size(); i++) {
        index_vector[i] = (i + index) % table_size;
    }
    return index_vector;
}

void write_word(vector<vector<int>>& Table, int index, vector<int> new_word) {
    vector<int> index_vector = get_index_vector(index, Table.size());
    for (int i = 0; i < Table.size(); i++) {
        Table[index_vector[i]][index] = new_word[i];
    }
}

void write_col(vector<vector<int>>& Table, int index, vector<int> new_col) {
    vector<int> index_vector = get_index_vector(index, Table.size());
    for (int i = 0; i < Table.size(); i++) {
        Table[index_vector[i]][i] = new_col[i];
    }
}

vector<int> read_word(vector<vector<int>>& Table, int index) {
    vector<int> index_vector = get_index_vector(index, Table.size());
    vector<int> word(Table.size(), 0);
    for (int i = 0; i < Table.size(); i++) {
        word[i] = Table[index_vector[i]][index];
    }
    return word;
}

vector<int> read_col(vector<vector<int>>& Table, int index) {
    vector<int> index_vector = get_index_vector(index, Table.size());
    vector<int> col(Table.size(), 0);
    for (int i = 0; i < Table.size(); i++) {
        col[i] = Table[index_vector[i]][i];
    }
    return col;
}

vector<int> calculate_word(vector<int> word1, vector<int> word2, bool(*f)(bool, bool)) {
    vector<int> result_word(word1.size(), 0);
    for (int i = 0; i < word1.size(); i++) {
        result_word[i] = f(word1[i], word2[i]);
    }
    return result_word;
}

vector<int> Dec_To_Bin_3(int dec) {
    vector<int> bin(3, 0);
    for (int i = 2; i >= 0; i--) {
        bin[i] = dec % 2;
        dec /= 2;
    }
    return bin;
}

vector<int> Dec_To_Bin_16(int dec) {
    vector<int> bin(16, 0);
    for (int i = 15; i >= 0; i--) {
        bin[i] = dec % 2;
        dec /= 2;
    }
    return bin;
}

int Pryam_To_Dec(vector<int> bin) {
    int integer = 0;
    for (int i = bin.size(); i > 0; i--) {
        integer += bin[i - 1] * pow(2, bin.size() - i);
    }
    return integer;
}

vector<int> find_words(vector<vector<int>>& Table, int decV) {
    vector<int> word_index_list;
    vector<int> binV = Dec_To_Bin_3(decV);
    for (int word_index = 0; word_index < Table.size(); word_index++) {
        vector<int> i_vec = get_index_vector(word_index, Table.size());
        if (Table[i_vec[0]][word_index] == binV[0] && Table[i_vec[1]][word_index] == binV[1] && Table[i_vec[2]][word_index] == binV[2]) {
            word_index_list.push_back(word_index);
        }
    }
    return word_index_list;
}

vector<int> Bin_Sum(vector<int> A, vector<int> B) {
    vector<int> S;
    A.insert(A.begin(), 0);
    B.insert(B.begin(), 0);

    int sum = 0;
    for (int i = A.size() - 1; i >= 0; i--) {
        S.insert(S.begin(), (sum + A[i] + B[i]) % 2);
        sum = (sum + A[i] + B[i]) > 1 ? 1 : 0;
    }

    while (S.size() < 5) {
        S.insert(S.begin(), 0);
    }

    return S;
}

vector<int> sum_fields(vector<vector<int>>& Table, int decV) {
    vector<int> words_index_list = find_words(Table, decV);
    vector<int> V = Dec_To_Bin_3(decV);

    for (int i = 0; i < words_index_list.size(); ++i) {
        vector<int> word = read_word(Table, words_index_list[i]);
        vector<int> A(word.begin() + 3, word.begin() + 7);
        vector<int> B(word.begin() + 7, word.begin() + 11);
        vector<int> S = Bin_Sum(A, B);

        vector<int> new_word;
        new_word.insert(new_word.end(), V.begin(), V.begin() + 3);
        new_word.insert(new_word.end(), A.begin(), A.end());
        new_word.insert(new_word.end(), B.begin(), B.end());
        new_word.insert(new_word.end(), S.begin(), S.end());
        write_word(Table, words_index_list[i], new_word);
    }
    return words_index_list;
}

bool compare_two_words(vector<int> a, vector<int> s) {
    bool g = 0;
    bool l = 0;
    for (int i = 0; i < a.size(); i++) {
        bool newg = g || (!a[i] && s[i] && !l);
        bool newl = l || (a[i] && !s[i] && !g);
        g = newg;
        l = newl;
        if (g > l) {
            return false;
        }
        if (g < l) {
            return true;
        }
    }
    if (g == l == 0) {
        return true;
    }
}

vector<int> find_words_in_range(vector<vector<int>>& Table, int decDown, int decUp) {
    vector<int> words_index_list;
    vector<int> binDown = Dec_To_Bin_16(decDown);
    vector<int> binUp = Dec_To_Bin_16(decUp);
    for (int i = 0; i < Table.size(); i++) {
        vector<int> word = read_word(Table, i);
        if (compare_two_words(word, binDown) && compare_two_words(binUp, word)) {
            words_index_list.push_back(i);
        }
    }
    return words_index_list;
}
