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

bool Conjunction(bool a, bool b) { return (a && b); }
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
            case '&': result = Conjunction(x, y); break;
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
    int total = 1 << n;
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

form NormForm(vector<bool> strtruthtable, vector<char> variables) {
    form onesdnf;

    for (int i = 0; i < variables.size(); i++) {
        onesdnf.variables.push_back(variables[i]);
        onesdnf.values.push_back(strtruthtable[i]);
    }
    return onesdnf;
}

vector<form> SDNF(vector<vector<bool>> truthtable, vector<char> variables) {
    vector<form> SDNF;
    for (int i = 0; i < truthtable.size(); i++) {
        if (truthtable[i][truthtable[i].size() - 1]) {
            form onesdnf = NormForm(truthtable[i], variables);
            SDNF.push_back(onesdnf);
        }
    }
    return SDNF;
}

vector<form> SKNF(vector<vector<bool>> truthtable, vector<char> variables) {
    vector<form> SKNF;
    for (int i = 0; i < truthtable.size(); i++) {
        if (!truthtable[i][truthtable[i].size() - 1]) {
            form onesknf = NormForm(truthtable[i], variables);
            SKNF.push_back(onesknf);
        }
    }
    return SKNF;
}

string print_one_sdnf(form onesdnf) {
    string sdnf;
    sdnf += "(";
    for (int i = 0; i < onesdnf.variables.size(); i++) {
        if (onesdnf.values[i]) {
            sdnf += onesdnf.variables[i];
        }
        else {
            sdnf += "(!";
            sdnf += onesdnf.variables[i];
            sdnf += ")";
        }

        if (i != onesdnf.variables.size() - 1) {
            sdnf += "&";
        }
    }
    sdnf += ")";
    return sdnf;
}

string print_sdnf(vector<form> SDNF) {
    string sdnf;
    for (int i = 0; i < SDNF.size(); i++) {
        string onesndf = print_one_sdnf(SDNF[i]);
        sdnf += onesndf;
        if (i != SDNF.size() - 1) {
            sdnf += " | ";
        }
    }
    return sdnf;
}

string print_one_sknf(form onesknf) {
    string sknf;
    sknf += "(";
    for (int i = 0; i < onesknf.variables.size(); i++) {
        if (!onesknf.values[i]) {
            sknf += onesknf.variables[i];
        }
        else {
            sknf += "(!";
            sknf += onesknf.variables[i];
            sknf += ")";
        }

        if (i != onesknf.variables.size() - 1) {
            sknf += "|";
        }
    }
    sknf += ")";
    return sknf;
}

string print_sknf(vector<form> SKNF) {
    string sknf;
    for (int i = 0; i < SKNF.size(); i++) {
        string onesknf = print_one_sknf(SKNF[i]);
        sknf += onesknf;
        if (i != SKNF.size() - 1) {
            sknf += " & ";
        }
    }
    return sknf;
}

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

bool check_form_in_set(set<form> set_of_forms, form a) {
    for (form form : set_of_forms) {
        if (check_normform_equal(form, a)) {
            return true;
        }
    }
    return false;
}

vector<form> merge(vector<form> normform) {
    set<int> index_used_sdnf;
    vector<form> new_sdnf;
    for (int i = 0; i < normform.size(); i++) {
        form part1 = normform[i];
        for (int j = i + 1; j < normform.size(); j++) {
            form part2 = normform[j];
            if (check_diff_var(part1, part2)) {
                form new_part;
                int index = find_diff_var(part1, part2);
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

    return true;
}

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

vector<form> merging_sdnf(vector<form> sdnf) {
    vector<form> current_sdnf = merging(sdnf);
    vector<char> all_vars = collect_vars(current_sdnf);
    current_sdnf = remove_redundant_implicants(current_sdnf, all_vars, 1);
    return current_sdnf;
}

vector<form> merging_sknf(vector<form> sknf) {
    vector<form> current_sknf = merging(sknf);
    vector<char> all_vars = collect_vars(current_sknf);
    current_sknf = remove_redundant_implicants(current_sknf, all_vars, 0);
    return current_sknf;
}

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

string print_table_merging(vector<vector<form>> table, int col_width, int oper) {
    vector<string> printtable(table.size(),"");
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
            printtable[i] += cell_content;
        }
        printtable[i] += '\n';
        for (int k = 0; k < table[i].size(); k++) {
            for (int m = 0; m < col_width; m++) {
                printtable[i] += '-';
            }
        }
        printtable[i] += '\n';
    }
    string tablestring;
    for (int i = 0; i < printtable.size(); i++) {
        tablestring += printtable[i];
    }
    return tablestring;
}

vector<vector<bool>> BuildKMapFromTruthTable(const vector<vector<bool>>& truthTable, const vector<char>& variables) {
    int n = variables.size();

    int rowVarsCount = n / 2;
    int colVarsCount = n - rowVarsCount;

    vector<char> rowVars(variables.begin(), variables.begin() + rowVarsCount);
    vector<char> colVars(variables.begin() + rowVarsCount, variables.end());

    int rowCount = 1 << rowVarsCount;
    int colCount = 1 << colVarsCount;

    vector<vector<bool>> kmap(rowCount, vector<bool>(colCount, 0));

    for (int row = 0; row < rowCount; ++row) {
        int rowGray = row ^ (row >> 1);

        for (int col = 0; col < colCount; ++col) {
            int colGray = col ^ (col >> 1);

            map<char, bool> assignment;

            for (int i = 0; i < rowVarsCount; ++i)
                assignment[rowVars[i]] = (rowGray >> (rowVarsCount - 1 - i)) & 1;

            for (int i = 0; i < colVarsCount; ++i)
                assignment[colVars[i]] = (colGray >> (colVarsCount - 1 - i)) & 1;

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
                    kmap[row][col] = truthTable[r].back();
                    break;
                }
            }
        }
    }

    return kmap;
}

string printKMap(const vector<vector<bool>>& kmap, int rowVars, int colVars) {
    int rowCount = kmap.size();
    int colCount = kmap[0].size();
    const int cellWidth = colVars + 2;
    vector<string> printkmap(rowCount,"");

    printkmap[0] += " ";
    for (int col = 0; col < colCount; ++col) {
        int gray = col ^ (col >> 1);
        string bits;
        for (int i = colVars - 1; i >= 0; --i) {
            bits += ((gray >> i) & 1) + '0';
        }
        printkmap[0] += "|";
        printkmap[0] += bits;
    }
    printkmap[0] += "|";
    printkmap[0] += "\n";
    printkmap[0] += " ";

    for (int i = 0; i < cellWidth; i++) {
        printkmap[0] += '-';
    }
    printkmap[0] += "+";
    for (int col = 0; col < colCount; ++col) {
        for (int i = 0; i < cellWidth; i++) {
            printkmap[0] += '-';
        }
        printkmap[0] += "+";
    }
    printkmap[0] += "\n";
    printkmap[0] += " ";

    for (int row = 0; row < rowCount; ++row) {
        int grayRow = row ^ (row >> 1);
        string rowBits;
        for (int i = rowVars - 1; i >= 0; --i) {
            rowBits += ((grayRow >> i) & 1) + '0';
        }

        printkmap[row] += rowBits;
        printkmap[row] += " |";

        for (int col = 0; col < colCount; ++col) {
            if (!kmap[row][col]) {
                printkmap[row] += "1";
                printkmap[row] += " |";
            }
            else {
                printkmap[row] += "0";
                printkmap[row] += " |";
            }
        }
        printkmap[row] += "\n";
        printkmap[row] += " ";

        for (int i = 0; i < cellWidth; i++) {
            printkmap[row] += '-';
        }
        printkmap[row] += "+";
        for (int col = 0; col < colCount; ++col) {
            for (int i = 0; i < cellWidth; i++) {
                printkmap[row] += '-';
            }
            printkmap[row] += "+";
        }
        printkmap[row] += "\n";
        printkmap[row] += " ";
    }

    string tablestring;
    for (int i = 0; i < printkmap.size(); i++) {
        tablestring += printkmap[i];
    }
    return tablestring;
}

int toGray(int num) {
    return num ^ (num >> 1);
}

bool groupAllEquals(const vector<vector<bool>>& kmap, int r, int c, int h, int w, int oper) {
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

form groupToForm(int rows, int cols, int r, int c, int h, int w, const vector<char>& rowVars, const vector<char>& colVars) {
    int rowVarsCount = rowVars.size();
    int colVarsCount = colVars.size();

    vector<char> vars;
    vector<bool> vals;

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

vector<form> minimize_KMap(const vector<vector<bool>>& kmap, const vector<char>& variables, int oper) {
    int varCount = variables.size();
    int rowVarsCount = varCount / 2;
    int colVarsCount = varCount - rowVarsCount;

    vector<char> rowVars(variables.begin(), variables.begin() + rowVarsCount);
    vector<char> colVars(variables.begin() + rowVarsCount, variables.end());

    int rows = kmap.size();
    int cols = kmap[0].size();

    set<pair<int, int>> covered;
    vector<form> result;

    for (int h = rows; h >= 1; h >>= 1) {
        for (int w = cols; w >= 1; w >>= 1) {
            for (int r = 0; r < rows; r++) {
                for (int c = 0; c < cols; c++) {
                    if (!groupAllEquals(kmap, r, c, h, w, oper)) continue;

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

                    form f = groupToForm(rows, cols, r, c, h, w, rowVars, colVars);
                    result.push_back(f);

                    for (int i = 0; i < h; ++i)
                        for (int j = 0; j < w; ++j)
                            covered.insert({ (r + i) % rows, (c + j) % cols });
                }
            }
        }
    }

    return result;
}
