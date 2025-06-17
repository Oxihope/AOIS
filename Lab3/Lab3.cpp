#include <iostream>
#include <iomanip>
#include <algorithm>
#include <set>
#include <map>
#include "LogicForms.h"

//Минимизация расчетным методом 
//Поиск индекса разнознаковой переменной
int find_diff_var(form form1, form form2) {
    int index = -1;
    for (int i = 0; i < form1.variables.size(); i++) {
        if (form1.values[i] != form2.values[i]) {
            index = i;
            return index;
        }
    }
    return index;
}
//Проверка наличия разнознаковой переменной
bool check_diff_var(form form1, form form2) {
    int count_diff = 0;
    bool diff_var = false;
    bool diff_val = false;
    if (form1.variables.size() == form2.variables.size()) {
        for (int i = 0; i < form1.variables.size(); i++) {
            if (form1.variables[i] == form2.variables[i]) {
                if (form1.values[i] != form2.values[i]) {
                    diff_val = true;
                    count_diff++;
                }
            }
            else {
                diff_var = true;
            }
        }
        if ((diff_var == false) && (diff_val == true) && (count_diff == 1)) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}
//Проверка на равенство форм
bool check_normform_equal(form form1, form form2) {
    if (form1.variables.size() != form2.variables.size()) {
        return false;
    }

    for (int i = 0; i < form1.variables.size(); i++) {
        if (form1.variables[i] != form2.variables[i]) {
            return false;
        }
    }

    for (int i = 0; i < form1.variables.size(); i++) {
        if (form1.values[i] != form2.values[i]) {
            return false;
        }
    }

    return true;
}
//Проверка на наличие формы во множестве
bool check_form_in_set(set<form> set_of_forms, form a) {
    for (form form : set_of_forms) {
        if (check_normform_equal(form, a)) {
            return true;
        }
    }
    return false;
}
//Объединение
vector<form> merge(vector<form> normform) {
    set<int> index_used_sdnf;
    vector<form> new_sdnf;
    for (int i = 0; i < normform.size(); i++) {
        form part1 = normform[i];
        for (int j = i + 1; j < normform.size(); j++) {
            form part2 = normform[j];
            if (check_diff_var(part1, part2)) {
                form new_part;
                int index = find_diff_var(part1,part2);
                for (int k = 0; k < part1.variables.size(); k++) {
                    if (k != index) {
                        new_part.variables.push_back(part1.variables[k]);
                        new_part.values.push_back(part1.values[k]);
                    }
                }
                new_sdnf.push_back(new_part);
                index_used_sdnf.insert(i);
                index_used_sdnf.insert(j);
            }
        }
    }

    for (int i = 0; i < normform.size(); i++) {
        if (index_used_sdnf.find(i) == index_used_sdnf.end()) {
            new_sdnf.push_back(normform[i]);
        }
    }

    set<form> unique_forms;
    vector<form> result;
    for (int i = 0; i < new_sdnf.size(); i++) {
        if (!check_form_in_set(unique_forms, new_sdnf[i])) {
            result.push_back(new_sdnf[i]);
            unique_forms.insert(new_sdnf[i]);
        }
    }
    
    return result;
}
//Проверка на равенство векторов форм
bool check_normform_vector_equal(vector<form> sdnf1, vector<form> sdnf2) {
    if (sdnf1.size() != sdnf2.size()) {
        return false;
    }

    for (int i = 0; i < sdnf1.size(); i++) {
        if (!check_normform_equal(sdnf1[i], sdnf2[i])) {
            return false;
        }
    }

    return true;
}
//Минимизация расчетным методом
vector<form> merging(vector<form> normform) {
    vector<form> current_sdnf = normform;
    while (true) {
        vector<form> new_sdnf = merge(current_sdnf);
        if (check_normform_vector_equal(new_sdnf, current_sdnf)) {
            break;
        }
        current_sdnf = new_sdnf;
    }
    return current_sdnf;
}
//Сбор всех переменным
vector<char> collect_vars(vector<form> sdnf) {
    vector<char> Variables;
    for (int i = 0; i < sdnf.size(); i++) {
        for (int j = 0; j < sdnf[i].variables.size(); j++) {
            Variables.push_back(sdnf[i].variables[j]);
        }
    }
    vector<char> all_vars;
    for (char ch : Variables) {
        if (find(all_vars.begin(), all_vars.end(), ch) == all_vars.end()) {
            all_vars.push_back(ch);
        }
    }
    return all_vars;
}
//Подсчет СДНФ
bool evaluate_sdnf(const vector<form>& sdnf, const map<char, bool>& vars_val) {
    for (auto& implicant : sdnf) {
        bool implicant_val = true;
        for (int i = 0; i < implicant.variables.size(); i++) {
            char var = implicant.variables[i];
            int val = implicant.values[i];
            if (vars_val.at(var) != val) {
                implicant_val = false;
                break;
            }
        }
        if (implicant_val) {
            return 1;
        }
    }
    return 0;
}
//Подсчет СКНФ
bool evaluate_sknf(const vector<form>& sknf, const map<char, bool>& vars_val) {
    for (auto& implicant : sknf) {
        bool implicant_val = false;
        for (int i = 0; i < implicant.variables.size(); i++) {
            char var = implicant.variables[i];
            int val = implicant.values[i];
            if (vars_val.at(var) != val) {
                implicant_val = true;
                break;
            }
        }
        if (!implicant_val) {
            return 0;
        }
    }
    return 1;
}
//Проверка на лишнюю импликанту
bool is_redundant(const vector<form>& normform, int index, const vector<char>& all_vars, int oper) {
    vector<form> normform_without;
    for (int i = 0; i < normform.size(); i++) {
        if (i != index)
            normform_without.push_back(normform[i]);
    }

    int num_vars = all_vars.size();
    int total_combinations = 1 << num_vars;

    for (int raw_index = 0; raw_index < total_combinations; raw_index++) {
        map<char, bool> vars_val;
        for (int i = 0; i < num_vars; i++) {
            vars_val[all_vars[i]] = (raw_index >> i) & 1;
        }

        bool val_with;
        bool val_without;

        if (oper == 1) {
            val_with = evaluate_sdnf(normform, vars_val);
            val_without = evaluate_sdnf(normform_without, vars_val);
        }

        if (oper == 0) {
            val_with = evaluate_sknf(normform, vars_val);
            val_without = evaluate_sknf(normform_without, vars_val);
        }

        if (val_with != val_without) {
            return false;
        }
    }

    // Если для всех комбинаций значения совпали — импликанта лишняя
    return true;
}
//Удаление лишних импликант
vector<form> remove_redundant_implicants(const vector<form>& normform, const vector<char>& all_vars, int oper) {
    vector<form> result = normform;

    bool changed = true;
    while (changed) {
        changed = false;
        for (int i = 0; i < (int)result.size(); i++) {
            if (is_redundant(result, i, all_vars, oper)) {
                result.erase(result.begin() + i);
                changed = true;
                break;
            }
        }
    }
    return result;
}
//Минимизация СДНФ расчётным методом с этапом склеивания
vector<form> merging_sdnf(vector<form> sdnf) {
    vector<form> current_sdnf = merging(sdnf);
    cout << endl << "МинДНФ с лишними импликантами: ";
    print_sdnf(current_sdnf);
    vector<char> all_vars = collect_vars(current_sdnf);
    current_sdnf = remove_redundant_implicants(current_sdnf, all_vars, 1);
    return current_sdnf;
}
//Минимизация СКНФ расчётным методом с этапом склеивания
vector<form> merging_sknf(vector<form> sknf) {
    vector<form> current_sknf = merging(sknf);
    cout << endl << "МинКНФ с лишними импликантами: ";
    print_sknf(current_sknf);
    vector<char> all_vars = collect_vars(current_sknf);
    current_sknf = remove_redundant_implicants(current_sknf, all_vars, 0);
    return current_sknf;
}

//Минимизация расчетно-табличным методом
//Проверка вхождения формы строки в форму столбца
bool check_form(form form_row, form form_column) {
    for (int i = 0; i < form_row.variables.size(); i++) {
        char var = form_row.variables[i];
        int val = form_row.values[i];

        bool found = false;
        for (int j = 0; j < form_column.variables.size(); j++) {
            if (form_column.variables[j] == var) {
                if (form_column.values[j] != val) {
                    return false;
                }
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
    }
    return true;
}
//Минимизация расчетно-табличным методом
vector<vector<form>> table_merging(vector<form> normform) {
    vector<vector<form>> table;

    vector<form> first_row;
    form empty_form;
    first_row.push_back(empty_form);
    for (int i = 0; i < normform.size(); i++) {
        form form;
        form.variables = normform[i].variables;
        form.values = normform[i].values;
        first_row.push_back(form);
    }
    table.push_back(first_row);

    vector<form> merged_form = merging(normform);

    for (int i = 0; i < merged_form.size(); i++) {
        vector<form> row;
        form form;
        form.variables = merged_form[i].variables;
        form.values = merged_form[i].values;
        row.push_back(form);
        table.push_back(row);
    }

    for (int i = 1; i < table.size(); i++) {
        for (int j = 1; j < table[0].size(); j++) {
            if (check_form(table[i][0], table[0][j])) {
                form form;
                form.variables.push_back('1');
                form.values.push_back(true);
                table[i].push_back(form);
            }
            else {
                form form;
                form.variables.push_back('0');
                form.values.push_back(false);
                table[i].push_back(form);
            }
        }
    }

    return table;
}
//Вывод таблицы склеивания
void print_table_merging(vector<vector<form>> table, int col_width, int oper) {
    for (int i = 0; i < table.size(); i++) {
        for (int j = 0; j < table[i].size(); j++) {

            string cell_content;

            if (i == 0 && j == 0) {
                cell_content = " |";
            }
            else {
                if (table[i][j].variables.size() == 1 &&
                    (table[i][j].variables[0] == '0' || table[i][j].variables[0] == '1')) {

                    if (table[i][j].variables[0] == '0') {
                        cell_content = " |";
                    }
                    else {
                        cell_content = "X|";
                    }

                }
                else {
                    cell_content += "(";
                    for (int k = 0; k < table[i][j].variables.size(); k++) {
                        if (table[i][j].values[k]) {
                            cell_content += table[i][j].variables[k];
                        }
                        else {
                            cell_content += "(!";
                            cell_content += table[i][j].variables[k];
                            cell_content += ")";
                        }
                        if (k != table[i][j].variables.size() - 1) {
                            if (oper == 0) {
                                cell_content += "|";
                            }
                            if (oper == 1) {
                                cell_content += "&";
                            }
                        }
                    }
                    cell_content += ")|";
                }
            }
            cout << setw(col_width) << cell_content;
        }
        cout << endl;
        for (int k = 0; k < table[i].size(); k++) {
            for (int m = 0; m < col_width; m++) {
                cout << "-";
            }
        }
        cout << endl;
    }
}

//Карта Карно
//Построение карты Карно
vector<vector<int>> BuildKMapFromTruthTable(const vector<vector<bool>>& truthTable, const vector<char>& variables) {
    int n = variables.size();

    int rowVarsCount = n / 2;
    int colVarsCount = n - rowVarsCount;

    vector<char> rowVars(variables.begin(), variables.begin() + rowVarsCount);
    vector<char> colVars(variables.begin() + rowVarsCount, variables.end());

    int rowCount = 1 << rowVarsCount;
    int colCount = 1 << colVarsCount;

    vector<vector<int>> kmap(rowCount, vector<int>(colCount, 0));

    for (int row = 0; row < rowCount; ++row) {
        int rowGray = row ^ (row >> 1);

        for (int col = 0; col < colCount; ++col) {
            int colGray = col ^ (col >> 1);

            //Составим полную карту переменных
            map<char, bool> assignment;

            for (int i = 0; i < rowVarsCount; ++i)
                assignment[rowVars[i]] = (rowGray >> (rowVarsCount - 1 - i)) & 1;

            for (int i = 0; i < colVarsCount; ++i)
                assignment[colVars[i]] = (colGray >> (colVarsCount - 1 - i)) & 1;

            //Найдём строку в truthTable, которая соответствует этому набору
            for (int r = 0; r < truthTable.size(); r++) {
                bool match = true;
                for (int i = 0; i < n; i++) {
                    char var = variables[i];
                    if (truthTable[r][i] != assignment[var]) {
                        match = false;
                        break;
                    }
                }
                if (match) {
                    kmap[row][col] = truthTable[r].back(); //результат
                    break;
                }
            }
        }
    }

    return kmap;
}
//Вывод карты Карно
void printKMap(const vector<vector<int>>& kmap, int rowVars, int colVars) {
    int rowCount = kmap.size();
    int colCount = kmap[0].size();
    const int cellWidth = colVars + 2;

    cout << setw(cellWidth) << " ";
    for (int col = 0; col < colCount; ++col) {
        int gray = col ^ (col >> 1);
        string bits;
        for (int i = colVars - 1; i >= 0; --i) {
            bits += ((gray >> i) & 1) + '0';
        }
        cout << "|" << setw(cellWidth) << bits;
    }
    cout << "|" << endl;

    cout << string(cellWidth, '-') << "+";
    for (int col = 0; col < colCount; ++col) {
        cout << string(cellWidth, '-') << "+";
    }
    cout << endl;

    for (int row = 0; row < rowCount; ++row) {
        int grayRow = row ^ (row >> 1);
        string rowBits;
        for (int i = rowVars - 1; i >= 0; --i) {
            rowBits += ((grayRow >> i) & 1) + '0';
        }

        cout << setw(cellWidth - 1) << rowBits << " |";

        for (int col = 0; col < colCount; ++col) {
            cout << setw(cellWidth - 1) << kmap[row][col] << " |";
        }
        cout << endl;

        cout << string(cellWidth, '-') << "+";
        for (int col = 0; col < colCount; ++col) {
            cout << string(cellWidth, '-') << "+";
        }
        cout << endl;
    }
}
//Получение Грей-кода
int toGray(int num) {
    return num ^ (num >> 1);
}
//Проверка на равенство всей подгруппы значению oper
bool groupAllEquals(const vector<vector<int>>& kmap, int r, int c, int h, int w, int oper) {
    int rows = kmap.size();
    int cols = kmap[0].size();
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            int rr = (r + i) % rows;
            int cc = (c + j) % cols;
            if (kmap[rr][cc] != oper) return false;
        }
    }
    return true;
}
//Построение формулы из прямоугольной группы
form groupToForm(int rows, int cols, int r, int c, int h, int w, const vector<char>& rowVars, const vector<char>& colVars) {
    int rowVarsCount = rowVars.size();
    int colVarsCount = colVars.size();

    vector<char> vars;
    vector<bool> vals;

    //Перебираем все ячейки в группе, собираем множество значений переменных
    set<vector<bool>> assignments;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            int rr = (r + i) % rows;
            int cc = (c + j) % cols;
            int rowBin = toGray(rr);
            int colBin = toGray(cc);

            vector<bool> full;
            for (int b = rowVarsCount - 1; b >= 0; b--)
                full.push_back((rowBin >> b) & 1);
            for (int b = colVarsCount - 1; b >= 0; b--)
                full.push_back((colBin >> b) & 1);

            assignments.insert(full);
        }
    }

    //Достаем неизменные переменные
    int totalVars = rowVarsCount + colVarsCount;
    for (int i = 0; i < totalVars; i++) {
        bool same = true;
        bool val = (*assignments.begin())[i];
        for (auto& a : assignments) {
            if (a[i] != val) {
                same = false;
                break;
            }
        }
        if (same) {
            vars.push_back(i < rowVarsCount ? rowVars[i] : colVars[i - rowVarsCount]);
            vals.push_back(val);
        }
    }

    return { vars, vals };
}
//Минимазация с помощью карты Карно
vector<form> Minimize_KMap(const vector<vector<int>>& kmap, const vector<char>& variables, int oper) {
    int varCount = variables.size();
    int rowVarsCount = varCount / 2;
    int colVarsCount = varCount - rowVarsCount;

    vector<char> rowVars(variables.begin(), variables.begin() + rowVarsCount);
    vector<char> colVars(variables.begin() + rowVarsCount, variables.end());

    int rows = kmap.size();
    int cols = kmap[0].size();

    set<pair<int, int>> covered;
    vector<form> result;

    //Перебираем все возможные размеры прямоугольников
    for (int h = rows; h >= 1; h >>= 1) {
        for (int w = cols; w >= 1; w >>= 1) {
            //Перебираем все возможные позиции прямоугольников
            for (int r = 0; r < rows; r++) {
                for (int c = 0; c < cols; c++) {
                    if (!groupAllEquals(kmap, r, c, h, w, oper)) continue;

                    //Проверяем, не покрыта ли уже вся группа
                    bool allCovered = true;
                    for (int i = 0; i < h; i++) {
                        for (int j = 0; j < w; j++) {
                            int rr = (r + i) % rows;
                            int cc = (c + j) % cols;
                            if (covered.count({ rr, cc }) == 0)
                                allCovered = false;
                        }
                    }
                    if (allCovered) continue;

                    //Добавляем формулу
                    form f = groupToForm(rows, cols, r, c, h, w, rowVars, colVars);
                    result.push_back(f);

                    //Помечаем покрытые ячейки
                    for (int i = 0; i < h; ++i)
                        for (int j = 0; j < w; ++j)
                            covered.insert({ (r + i) % rows, (c + j) % cols });
                }
            }
        }
    }

    return result;
}

int main() {
    setlocale(LC_ALL, "Russian");
    string input;
    vector<char> charVector;
    cout << endl << "Введите формулу: ";
    cin >> input;

    //Удаление >
    for (int i = 0; i < input.length(); i++) {
        if (input[i] == '-') {
            charVector.push_back(input[i]);
            if ((i + 1) < input.length()) {
                i++;
            }
        }
        else {
            charVector.push_back(input[i]);
        }
    }

    //Запись переменных
    vector<char> Variables;
    for (char ch : charVector) {
        if (Prior(ch) == 0 && find(Variables.begin(), Variables.end(), ch) == Variables.end()) {
            Variables.push_back(ch);
        }
    }
    sort(Variables.begin(), Variables.end());

    //Формирование дерева
    stack* begin = new stack;
    stack* root = begin;
    for (int i = 0; i < charVector.size(); i++) {
        begin = Prior_Action(begin, charVector[i]);
    }

    //Формирование списка переменных
    vector<char> VarAndOper;
    for (int i = 0; i < Variables.size(); i++) {
        switch (Variables[i]) {
        case 'a': VarAndOper.push_back('a'); break;
        case 'b': VarAndOper.push_back('b'); break;
        case 'c': VarAndOper.push_back('c'); break;
        case 'd': VarAndOper.push_back('d'); break;
        case 'e': VarAndOper.push_back('e'); break;
        }
    }

    //Формирование порядка операций
    vector<stack*> nodesInOrder;
    CollectNodes(root, nodesInOrder);
    for (int i = 0; i < nodesInOrder.size(); i++) {
        if (Prior(nodesInOrder[i]->info) >= 2) {
            VarAndOper.push_back(nodesInOrder[i]->info);
        }
    }

    //Потроение и вывод таблицы истинности
    vector<vector<bool>> TruthTable = BuildTruthTable(root, Variables);
    cout << "Таблица истинности:" << endl;
        //Вывод строки переменных и операций
    for (int i = 0; i < VarAndOper.size(); i++) {
        cout << VarAndOper[i] << " ";
        if (i == Variables.size() - 1) {
            cout << " ";
        }
        if (i == VarAndOper.size() - 2) {
            cout << " ";
        }
    }
    cout << endl;
        //Вывод таблицы истинности
    for (int i = 0; i < TruthTable.size(); i++) {
        for (int j = 0; j < TruthTable[i].size(); j++) {
            cout << TruthTable[i][j] << " ";
            if (j == TruthTable.size() - 1) {
                cout << " ";
            }
            if (j == TruthTable[i].size() - 2) {
                cout << " ";
            }
        }
        cout << endl;
    }

    //Построение и вывод СДНФ
    cout << endl << "СДНФ: ";
    vector<form> sdnf = SDNF(TruthTable, Variables);
    print_sdnf(sdnf);
    //Минимизация СДНФ расчетным методом
    vector<form> min_sdnf = merging_sdnf(sdnf);
    cout << endl << "Минимизированная ДНФ без лишних импликант: ";
    print_sdnf(min_sdnf);
    cout << endl;
    //Минимизация СДНФ табличным методом
    vector<vector<form>> table_sdnf = table_merging(sdnf);
    int col_width = Variables.size()*5+3;
    cout << endl;
    print_table_merging(table_sdnf,col_width, 1);
    cout << endl << "Результат: ";
    print_sdnf(min_sdnf);

    cout << endl;

    //Построение и вывод СКНФ
    cout << endl << "СКНФ: ";
    vector<form> sknf = SKNF(TruthTable, Variables);
    print_sknf(sknf);
    //Минимизация СКНФ расчетным методом
    vector<form> min_sknf = merging_sknf(sknf);
    cout << endl << "Минимизированная КНФ без лишних импликант: ";
    print_sknf(min_sknf);
    cout << endl;
    //Минимизация СКНФ табличным методом
    vector<vector<form>> table_sknf = table_merging(sknf);
    cout << endl;
    print_table_merging(table_sknf, col_width, 0);
    cout << endl << "Результат: ";
    print_sknf(min_sknf);

    cout << endl;

    //Карта карно
    int rowVars = Variables.size() / 2;
    int colVars = Variables.size() - rowVars;
    vector<vector<int>> kmap = BuildKMapFromTruthTable(TruthTable, Variables);
    printKMap(kmap, rowVars, colVars);
    //Построение МинДНФ с помощью карты Карно
    vector<form> minimized_sdnf = Minimize_KMap(kmap, Variables, 1);
    cout << "СДНФ картой Карно:" << endl;
    print_sdnf(min_sdnf);
    cout << endl;
    //Построение МинКНФ с помощью карты Карно
    vector<form> minimized_sknf = Minimize_KMap(kmap, Variables, 0);
    cout << "СКНФ картой Карно:" << endl;
    print_sknf(min_sknf);

    return 0;
}