#include <vector>
#include <string>

using namespace std;

int Prior(char a);

struct stack {
    char info;
    stack* prev = nullptr;
    stack* right = nullptr;
    stack* left = nullptr;
};

stack* Prior_Action(stack* current, char a);

bool Ñonjunction(bool a, bool b);
bool Disjunction(bool a, bool b);
bool Implication(bool a, bool b);
bool Equivalence(bool a, bool b);
bool Negation(bool a);

void CollectNodes(stack* node, vector<stack*>& nodes);

bool Result(stack* current, bool a, bool b, bool c, bool d, bool e, vector<bool>& results);

vector<vector<bool>> BuildTruthTable(stack* root, const vector<char>& variables);

vector<char> OneSDNF(vector<bool> strtruthtable, vector<char> variables);

vector<char> SDNF(vector<vector<bool>> truthtable, vector<char> variables);

vector<char> OneSKNF(vector<bool> strtruthtable, vector<char> variables);

vector<char> SKNF(vector<vector<bool>> truthtable, vector<char> variables);

vector<char> SDNF_number_form(vector<vector<bool>> truthtable);

vector<char> SKNF_number_form(vector<vector<bool>> truthtable);

int Index_form(vector<vector<bool>> truthtable, vector<int>& index_form_vec);