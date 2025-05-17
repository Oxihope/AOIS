#include <iostream>
#include <algorithm>
#include "LogicForms.h"

int main() {
    setlocale(LC_ALL, "Russian");
    string input;
    vector<char> charVector;
    cout << "Введите формулу: ";
    cin >> input;

    //замена -> на -
    for (int i = 0; i < input.length(); i++) {
        if (input[i] == '-') {
            charVector.push_back(input[i]);
            if ((i + 1) < input.length()) i++;
        }
        else {
            charVector.push_back(input[i]);
        }
    }

    //сбор переменных
    vector<char> Variables;
    for (char ch : charVector) {
        if (Prior(ch) == 0 && find(Variables.begin(), Variables.end(), ch) == Variables.end()) {
            Variables.push_back(ch);
        }
    }

    sort(Variables.begin(), Variables.end());

    //формирование дерева формулы
    stack* begin = new stack;
    stack* root = begin;
    for (int i = 0; i < charVector.size(); i++) {
        begin = Prior_Action(begin, charVector[i]);
    }

    vector<vector<bool>> TruthTable = BuildTruthTable(root, Variables);

    //формирование строки переменных и операций
    //строка переменных
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
    //строка операций в последовательности выполнения
    vector<stack*> nodesInOrder;
    CollectNodes(root, nodesInOrder);
    for (int i = 0; i < nodesInOrder.size(); i++) {
        if (Prior(nodesInOrder[i]->info) >= 2) {
            VarAndOper.push_back(nodesInOrder[i]->info);
        }
    }

    cout << "Таблица истинности:" << endl;

    //Вывод таблицы истинности
    //Вывод строки переменных и операций
    for (int i = 0; i < VarAndOper.size(); i++) {
        cout << VarAndOper[i] << " ";
        //отделение переменных
        if (i == Variables.size() - 1) {
            cout << " ";
        }
        //отделение результата
        if (i == VarAndOper.size() - 2) {
            cout << " ";
        }
    }
    cout << endl;
    //Вывод значений
    for (int i = 0; i < TruthTable.size(); i++) {
        for (int j = 0; j < TruthTable[i].size(); j++) {
            cout << TruthTable[i][j] << " ";
            //отделение переменных
            if (j == Variables.size() - 1) {
                cout << " ";
            }
            //отделение результата
            if (j == TruthTable[i].size() - 2) {
                cout << " ";
            }
        }
        cout << endl;
    }

    //Вывод СДНФ и СКНФ
    cout << endl << "СДНФ: ";
    vector<char> sdnf = SDNF(TruthTable, Variables);
    for (int i = 0; i < sdnf.size(); i++) {
        cout << sdnf[i];
    }
    cout << endl;
    cout << endl << "СКНФ: ";
    vector<char> sknf = SKNF(TruthTable, Variables);
    for (int i = 0; i < sknf.size(); i++) {
        cout << sknf[i];
    }

    cout << endl;
    //Вывод числовых форм
    cout << endl << "Числовые формы:" << endl;
    vector<char> sdnf_number_form = SDNF_number_form(TruthTable);
    for (int i = 0; i < sdnf_number_form.size(); i++) {
        cout << sdnf_number_form[i];
    }
    cout << endl;
    vector<char> sknf_number_form = SKNF_number_form(TruthTable);
    for (int i = 0; i < sknf_number_form.size(); i++) {
        cout << sknf_number_form[i];
    }

    cout << endl;
    //Вывод индексной формы
    vector<int> index_form_vec;
    int index_form = Index_form(TruthTable, index_form_vec);
    cout << endl << "Индексная форма: " << index_form << " - ";
    for (int i = 0; i < index_form_vec.size(); i++) {
        cout << index_form_vec[i];
    }

    cout << endl;
    return 0;
}
