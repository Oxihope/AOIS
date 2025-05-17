#include "LogicForms.h"

int Prior(char a) {
    if (a == '(') return 1;
    if (a == '&' || a == '|' || a == '-' || a == '~') return 2;
    if (a == ')') return 3;
    if (a == '!') return 4;
    return 0;
}

stack* Prior_Action(stack* current, char a) {
    int prior = Prior(a);
    if (prior == 0) {
        current->info = a;
        return current->prev;
    }
    if (prior == 1) {
        stack* t = new stack;
        t->prev = current;
        current->left = t;
        return t;
    }
    if (prior == 2) {
        current->info = a;
        stack* t = new stack;
        t->prev = current;
        current->right = t;
        return t;
    }
    if (prior == 3) {
        return current->prev;
    }
    if (prior == 4) {
        current->prev->info = a;
        return current;
    }
}

bool Ñonjunction(bool a, bool b) { return (a && b); }
bool Disjunction(bool a, bool b) { return (a || b); }
bool Implication(bool a, bool b) { return (!a || b); }
bool Equivalence(bool a, bool b) { return (a == b); }
bool Negation(bool a) { return !a; }

void CollectNodes(stack* node, vector<stack*>& nodes) {
    if (!node) return;
    CollectNodes(node->left, nodes);
    CollectNodes(node->right, nodes);
    nodes.push_back(node);
}

bool Result(stack* current, bool a, bool b, bool c, bool d, bool e, vector<bool>& results) {
    if (!current) return false;

    int prior = Prior(current->info);
    bool result = false;

    if (prior == 0) {
        switch (current->info) {
        case 'a': result = a; break;
        case 'b': result = b; break;
        case 'c': result = c; break;
        case 'd': result = d; break;
        case 'e': result = e; break;
        }
    }
    else {
        if (prior == 2) {
            bool x = Result(current->left, a, b, c, d, e, results);
            bool y = Result(current->right, a, b, c, d, e, results);
            switch (current->info) {
            case '&': result = Ñonjunction(x, y); break;
            case '|': result = Disjunction(x, y); break;
            case '-': result = Implication(x, y); break;
            case '~': result = Equivalence(x, y); break;
            }
        }
        else {
            if (prior == 4) {
                bool x = Result(current->left, a, b, c, d, e, results);
                result = !x;
            }
        }
        results.push_back(result);
    }

    return result;
}

vector<vector<bool>> BuildTruthTable(stack* root, const vector<char>& variables) {
    int n = variables.size();
    int total = 1 << n; //2^n
    vector<vector<bool>> table;

    for (int mask = 0; mask < total; mask++) {
        bool a = 0, b = 0, c = 0, d = 0, e = 0;
        vector<bool> line;

        for (int i = 0; i < n; i++) {
            bool val = (mask >> (n - 1 - i)) & 1;
            switch (variables[i]) {
            case 'a': a = val; break;
            case 'b': b = val; break;
            case 'c': c = val; break;
            case 'd': d = val; break;
            case 'e': e = val; break;
            }
        }
        vector<bool> stepresults;
        bool result = Result(root, a, b, c, d, e, stepresults);

        for (int i = 0; i < n; i++) {
            switch (variables[i]) {
            case 'a': line.push_back(a); break;
            case 'b': line.push_back(b); break;
            case 'c': line.push_back(c); break;
            case 'd': line.push_back(d); break;
            case 'e': line.push_back(e); break;
            }
        }
        line.insert(line.end(), stepresults.begin(), stepresults.end());

        table.push_back(line);
    }

    return table;
}

vector<char> OneSDNF(vector<bool> strtruthtable, vector<char> variables) {
    vector<char> onestrsdnf;
    onestrsdnf.push_back('(');

    for (int i = 0; i < variables.size(); i++) {
        if (strtruthtable[i]) {
            onestrsdnf.push_back(variables[i]);
        }
        else {
            onestrsdnf.push_back('(');
            onestrsdnf.push_back('!');
            onestrsdnf.push_back(variables[i]);
            onestrsdnf.push_back(')');
        }
        onestrsdnf.push_back('&');
    }
    onestrsdnf[onestrsdnf.size() - 1] = ')';

    return onestrsdnf;
}

vector<char> SDNF(vector<vector<bool>> truthtable, vector<char> variables) {
    vector<char> strsdnf;
    for (int i = 0; i < truthtable.size(); i++) {
        if (truthtable[i][truthtable[i].size() - 1]) {
            vector<char> onestrsdnf = OneSDNF(truthtable[i], variables);
            strsdnf.insert(strsdnf.end(), onestrsdnf.begin(), onestrsdnf.end());
            strsdnf.push_back(' ');
            strsdnf.push_back('|');
            strsdnf.push_back(' ');
        }
    }
    strsdnf.pop_back();
    strsdnf.pop_back();
    strsdnf.pop_back();
    return strsdnf;
}

vector<char> OneSKNF(vector<bool> strtruthtable, vector<char> variables) {
    vector<char> onestrsdnf;
    onestrsdnf.push_back('(');

    for (int i = 0; i < variables.size(); i++) {
        if (strtruthtable[i]) {
            onestrsdnf.push_back('(');
            onestrsdnf.push_back('!');
            onestrsdnf.push_back(variables[i]);
            onestrsdnf.push_back(')');
        }
        else {
            onestrsdnf.push_back(variables[i]);
        }
        onestrsdnf.push_back('|');
    }
    onestrsdnf[onestrsdnf.size() - 1] = ')';

    return onestrsdnf;
}

vector<char> SKNF(vector<vector<bool>> truthtable, vector<char> variables) {
    vector<char> strsdnf;
    for (int i = 0; i < truthtable.size(); i++) {
        if (!truthtable[i][truthtable[i].size() - 1]) {
            vector<char> onestrsdnf = OneSKNF(truthtable[i], variables);
            strsdnf.insert(strsdnf.end(), onestrsdnf.begin(), onestrsdnf.end());
            strsdnf.push_back(' ');
            strsdnf.push_back('&');
            strsdnf.push_back(' ');
        }
    }
    strsdnf.pop_back();
    strsdnf.pop_back();
    strsdnf.pop_back();
    return strsdnf;
}

vector<char> SDNF_number_form(vector<vector<bool>> truthtable) {
    vector<char> sdnf_number_form;
    sdnf_number_form.push_back('(');
    for (int i = 0; i < truthtable.size(); i++) {
        if (truthtable[i][truthtable[i].size() - 1]) {
            string number = to_string(i);
            for (char c : number) {
                sdnf_number_form.push_back(c);
            }
            sdnf_number_form.push_back(',');
        }
    }
    sdnf_number_form.pop_back();
    sdnf_number_form.push_back(')');
    sdnf_number_form.push_back(' ');
    sdnf_number_form.push_back('|');

    return sdnf_number_form;
}

vector<char> SKNF_number_form(vector<vector<bool>> truthtable) {
    vector<char> sknf_number_form;
    sknf_number_form.push_back('(');
    for (int i = 0; i < truthtable.size(); i++) {
        if (!truthtable[i][truthtable[i].size() - 1]) {
            string number = to_string(i);
            for (char c : number) {
                sknf_number_form.push_back(c);
            }
            sknf_number_form.push_back(',');
        }
    }
    sknf_number_form.pop_back();
    sknf_number_form.push_back(')');
    sknf_number_form.push_back(' ');
    sknf_number_form.push_back('&');

    return sknf_number_form;
}

int Index_form(vector<vector<bool>> truthtable, vector<int>& index_form_vec) {
    vector<int> bin;
    for (int i = 0; i < truthtable.size(); i++) {
        bin.push_back(truthtable[i][truthtable[i].size() - 1]);
    }

    index_form_vec = bin;

    int index = 0;
    while (index < bin.size() && bin[index] == 0) {
        index++;
    }
    if (index > 0) {
        bin.erase(bin.begin(), bin.begin() + index);
    }

    int integer = 0;
    for (int i = bin.size(); i > 0; i--) {
        integer += bin[i - 1] * pow(2, bin.size() - i);
    }

    return integer;
}