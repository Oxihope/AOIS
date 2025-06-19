#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

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
//Поулчение вектора индексов
vector<int> get_index_vector(int index, int table_size) {
    vector<int> index_vector(table_size,0);
    for (int i = 0; i < index_vector.size(); i++) {
        index_vector[i] = (i + index) % table_size;
    }
    return index_vector;
}
//Записать новое слово в таблицу по индексу
void write_word(vector<vector<int>> &Table, int index, vector<int> new_word) {
    vector<int> index_vector = get_index_vector(index, Table.size());
    for (int i = 0; i < Table.size(); i++) {
        Table[index_vector[i]][index] = new_word[i];
    }
}
//Записать новый адресный столбец в таблицу по индексу
void write_col(vector<vector<int>> &Table, int index, vector<int> new_col) {
    vector<int> index_vector = get_index_vector(index, Table.size());
    for (int i = 0; i < Table.size(); i++) {
        Table[index_vector[i]][i] = new_col[i];
    }
}
//Получить слово из таблицы по индексу
vector<int> read_word(vector<vector<int>> &Table, int index) {
    vector<int> index_vector = get_index_vector(index, Table.size());
    vector<int> word(Table.size(),0);
    for (int i = 0; i < Table.size(); i++) {
        word[i] = Table[index_vector[i]][index];
    }
    return word;
}
//Получить адресный столбец по индексу
vector<int> read_col(vector<vector<int>> &Table, int index) {
    vector<int> index_vector = get_index_vector(index, Table.size());
    vector<int> col(Table.size(), 0);
    for (int i = 0; i < Table.size(); i++) {
        col[i] = Table[index_vector[i]][i];
    }
    return col;
}
//Посчитать новое слово
vector<int> calculate_word(vector<int> word1, vector<int> word2, bool(*f)(bool,bool)) {
    vector<int> result_word(word1.size(),0);
    for (int i = 0; i < word1.size(); i++) {
        result_word[i] = f(word1[i], word2[i]);
    }
    return result_word;
}
//Перевод в бинарный код 3
vector<int> Dec_To_Bin_3(int dec) {
    vector<int> bin(3, 0);
    for (int i = 2; i >= 0; i--) {
        bin[i] = dec % 2;
        dec /= 2;
    }
    return bin;
}
//Перевод в бинарный код 16
vector<int> Dec_To_Bin_16(int dec) {
    vector<int> bin(16, 0);
    for (int i = 15; i >= 0; i--) {
        bin[i] = dec % 2;
        dec /= 2;
    }
    return bin;
}
//Перевод в десятичное число
int Pryam_To_Dec(vector<int> bin) {
    int integer = 0;
    for (int i = bin.size(); i > 0; i--) {
        integer += bin[i - 1] * pow(2, bin.size() - i);
    }
    return integer;
}
//Поиск слов с началом V
vector<int> find_words(vector<vector<int>> &Table, int decV) {
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
//Сумма полей А и В
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
//Сложение полей
vector<int> sum_fields(vector<vector<int>> &Table, int decV) {
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
//Сравнение двух слов (a>=s -> true)
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
//Поиск в заданном диапазоне
vector<int> find_words_in_range(vector<vector<int>> &Table, int decDown, int decUp) {
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
//Генереция рандомной матрицы
vector<vector<int>> GenerateBinaryMatrix(int rows, int cols) {
    vector<vector<int>> matrix(rows, vector<int>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = rand() % 2;
        }
    }
    return matrix;
}
//Вывод матрицы
void PrintMatrix(const vector<vector<int>>& matrix) {
    const int width = 3;

    cout << setw(width) << " ";
    for (int i = 0; i < matrix.size(); ++i) {
        cout << setw(width) << i;
    }
    cout << endl;

    cout << setw(width) << " " << setfill('-') << setw(width * matrix.size()) << "-" << setfill(' ') << endl;

    for (int i = 0; i < matrix.size(); ++i) {
        cout << setw(width - 1) << i << "|";
        for (int j = 0; j < matrix[i].size(); ++j) {
            cout << setw(width - 1) << matrix[i][j];
            if (i == j) {
                cout << "|";
            }
            else {
                cout << " ";
            }
        }
        cout << endl;
    }
}
int main() {
    setlocale(LC_ALL, "Russian");
    vector<vector<int>> matrix = GenerateBinaryMatrix(16, 16);
    PrintMatrix(matrix);
    bool does = true;
    while (does) {
        cout << "Выберете операцию:" << endl;
        cout << "1. Запись считывание разрядных столбцов и слов по индексу" << endl;
        cout << "2. Логические функции над словами" << endl;
        cout << "3. Поиск величин, заключенных в данном интервале" << endl;
        cout << "4. Сложение полей Aj и Bj в словах Sj, у которых Vj совпадает с заданным 0<= V <= 7" << endl;
        cout << "5. Выход" << endl;
        int operation;
        cin >> operation;
        switch (operation) {
        case 1: {
            cout << "1. Записать слово" << endl;
            cout << "2. Вывести слово" << endl;
            cout << "3. Записать адресный столбец" << endl;
            cout << "4. Вывести адресный стотлбец" << endl;
            int oper1;
            cin >> oper1;
            switch (oper1) {
            case 1: {
                cout << "Введите число от 0 до 65535" << endl;
                int word;
                cin >> word;
                vector<int> binword = Dec_To_Bin_16(word);
                cout << "Полученное слово: ";
                for (int i = 0; i < binword.size(); i++) {
                    cout << binword[i];
                }
                cout << endl << "Введите индекс, куда записать" << endl;
                int index;
                cin >> index;
                write_word(matrix, index, binword);
                PrintMatrix(matrix);
                break;
            }
            case 2: {
                cout << "Введите индекс" << endl;
                int index;
                cin >> index;
                vector<int> word = read_word(matrix, index);
                cout << "Полученное слово: ";
                for (int i = 0; i < word.size(); i++) {
                    cout << word[i];
                }
                cout << endl;
                break;
            }
            case 3: {
                cout << "Введите число от 0 до 65535" << endl;
                int word;
                cin >> word;
                vector<int> binword = Dec_To_Bin_16(word);
                cout << "Полученный столбец: ";
                for (int i = 0; i < binword.size(); i++) {
                    cout << binword[i];
                }
                cout << endl << "Введите индекс, куда записать" << endl;
                int index;
                cin >> index;
                write_col(matrix, index, binword);
                PrintMatrix(matrix);
                break;
            }
            case 4: {
                cout << "Введите индекс" << endl;
                int index;
                cin >> index;
                vector<int> word = read_col(matrix, index);
                cout << "Полученное слово: ";
                for (int i = 0; i < word.size(); i++) {
                    cout << word[i];
                }
                cout << endl;
                break;
            }
            }
            break;
        }
        case 2: {
            cout << "Введите индекс первого слова" << endl;
            int index1;
            cin >> index1;
            vector<int> word1 = read_word(matrix, index1);
            cout << "Полученное слово: ";
            for (int i = 0; i < word1.size(); i++) {
                cout << word1[i];
            }
            cout << endl;
            cout << "Введите индекс второго слова" << endl;
            int index2;
            cin >> index2;
            vector<int> word2 = read_word(matrix, index2);
            cout << "Полученное слово: ";
            for (int i = 0; i < word2.size(); i++) {
                cout << word2[i];
            }
            cout << endl;
            cout << "Выберете функцию" << endl;
            cout << "1. f2" << endl;
            cout << "2. f7" << endl;
            cout << "3. f8" << endl;
            cout << "4. f13" << endl;
            int func;
            cin >> func;
            vector<int> word3;
            switch (func) {
            case 1: {
                word3 = calculate_word(word1, word2, f2);
            }
            case 2: {
                word3 = calculate_word(word1, word2, f7);
            }
            case 3: {
                word3 = calculate_word(word1, word2, f8);
            }
            case 4: {
                word3 = calculate_word(word1, word2, f13);
            }
            }
            cout << "Полученное слово: ";
            for (int i = 0; i < word3.size(); i++) {
                cout << word3[i];
            }
            cout << endl;
            cout << "Введите индекс, куда записать поулченное слово" << endl;
            int index3;
            cin >> index3;
            write_word(matrix, index3, word3);
            PrintMatrix(matrix);
            break;
        }
        case 3: {
            cout << "Введите низ интервала (0-65534)" << endl;
            int down;
            cin >> down;
            cout << "Введите верх интервала (1-65535)" << endl;
            int up;
            cin >> up;
            vector<int> words_index_list = find_words_in_range(matrix, down, up);
            if (words_index_list.size() == 0) {
                cout << "Слов не найдено" << endl;
            }
            else {
                cout << "Найденные слова:" << endl;
                for (int i = 0; i < words_index_list.size(); i++) {
                    vector<int> word = read_word(matrix, words_index_list[i]);
                    int decword = Pryam_To_Dec(word);
                    cout << "Слово: ";
                    for (int j = 0; j < word.size(); j++) {
                        cout << word[j];
                    }
                    cout << " Десятичный вид: " << decword << endl;
                }
            }
            cout << endl;
        }
        case 4: {
            cout << "Ведите десятичный V (от 0 до 7):" << endl;
            int decV;
            cin >> decV;
            vector<int> binV = Dec_To_Bin_3(decV);
            cout << "Бинарный V: ";
            for (int i = 0; i < binV.size(); i++) {
                cout << binV[i];
            }
            cout << endl;
            vector<int> words_index_list = sum_fields(matrix, decV);
            cout << "Сложение произведено в словах с данными индексами: ";
            for (int i = 0; i < words_index_list.size(); i++) {
                cout << words_index_list[i] << " ";
            }
        }
        case 5:{
            does = false;
            break;
        }
        }
    }
}