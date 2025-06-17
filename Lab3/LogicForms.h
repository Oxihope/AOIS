#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <set>
#include <map>

using namespace std;

int Prior(char a);

struct stack {
    char info;
    stack* prev = nullptr;
    stack* right = nullptr;
    stack* left = nullptr;
};

struct form {
    vector<char> variables;
    vector<bool> values;

    bool operator<(const form& other) const {
        if (variables != other.variables) return variables < other.variables;
        return values < other.values;
    }
};

stack* Prior_Action(stack* current, char a);

bool Conjunction(bool a, bool b);
bool Disjunction(bool a, bool b);
bool Implication(bool a, bool b);
bool Equivalence(bool a, bool b);
bool Negation(bool a);

void CollectNodes(stack* node, vector<stack*>& nodes);

bool Result(stack* current, bool a, bool b, bool c, bool d, bool e, vector<bool>& results);

vector<vector<bool>> BuildTruthTable(stack* root, const vector<char>& variables);

form NormForm(vector<bool> strtruthtable, vector<char> variables);

vector<form> SDNF(vector<vector<bool>> truthtable, vector<char> variables);

vector<form> SKNF(vector<vector<bool>> truthtable, vector<char> variables);

string print_one_sdnf(form onesdnf);

string print_sdnf(vector<form> SDNF);

string print_one_sknf(form onesknf);

string print_sknf(vector<form> SKNF);


int find_diff_var(form form1, form form2);

bool check_diff_var(form form1, form form2);

bool check_normform_equal(form form1, form form2);

bool check_form_in_set(set<form> set_of_forms, form a);

vector<form> merge(vector<form> normform);

bool check_normform_vector_equal(vector<form> sdnf1, vector<form> sdnf2);

vector<form> merging(vector<form> normform);

vector<char> collect_vars(vector<form> sdnf);

bool evaluate_sdnf(const vector<form>& sdnf, const map<char, bool>& vars_val);

bool evaluate_sknf(const vector<form>& sknf, const map<char, bool>& vars_val);

bool is_redundant(const vector<form>& normform, int index, const vector<char>& all_vars, int oper);

vector<form> remove_redundant_implicants(const vector<form>& normform, const vector<char>& all_vars, int oper);

vector<form> merging_sdnf(vector<form> sdnf);

vector<form> merging_sknf(vector<form> sknf);

bool check_form(form form_row, form form_column);

vector<vector<form>> table_merging(vector<form> normform);

string print_table_merging(vector<vector<form>> table, int col_width, int oper);

vector<vector<bool>> BuildKMapFromTruthTable(const vector<vector<bool>>& truthTable, const vector<char>& variables);

string printKMap(const vector<vector<bool>>& kmap, int rowVars, int colVars);

int toGray(int num);

bool groupAllEquals(const vector<vector<bool>>& kmap, int r, int c, int h, int w, int oper);

form groupToForm(int rows, int cols, int r, int c, int h, int w, const vector<char>& rowVars, const vector<char>& colVars);

vector<form> minimize_KMap(const vector<vector<bool>>& kmap, const vector<char>& variables, int oper);
