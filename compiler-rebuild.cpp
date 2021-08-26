#include <iostream>

using namespace std;

string get_string(char x)
{
    string s(1, x);

    return s;  
}

// ------------------------------------------------------------------------------

template <typename T>
class stack {
    T stuff[20] = {};
    int size = 0;

public:
    void push(T element) {
        stuff[size++] = element;
    }

    T pop() {
        if (size <= 0) {
            return NULL;
        }
        return stuff[--size];
    }

    T top() {
        if (size <= 0) {
            return NULL;
        }
        return stuff[size - 1];
    }

    int stack_size() {
        return this->size;
    }
};

// --------------------------------- precedence table and helpers

struct op {     
    int order_of_p;
    string symbol;
    string print_instructions;
} neg, exp, mult, divide, add, subtract, lt, gt, ltet, gtet, net,
    et, _and, _or, assign, num, var, open_paren, closed_paren, _end, not_found;

const int NUM_OPS = 21;
op operators[NUM_OPS] = {
    {101, "@", "Neg\n"},
    {91, "^", "Exp\n"},
    {81, "*", "Mul\n"},
    {82, "/", "Div\n"},
    {71, "+", "Sum\n"},
    {72, "-", "Minus\n"},
    {61, "<", "LT\n"},
    {62, ">", "GT\n"},
    {63, "<=", "LE\n"},  //LE
    {64, ">=", "GE\n"},  //GE
    {65, "!=", "NE\n"},  // NE
    {66, "==", "EQ\n"},  // EQ
    {51, "&", "And\n"},
    {41, "|", "Or\n"},
    {31, "=", ""}, 
    {21, "N", "Push "},
    {22, "V", ""},
    {11, "(", ""},
    {12, ")", ""},
    {0, "$", ""},
    {-1, "X", ""}
};

int get_op_index(string op) {
    int index = 0;
    while (operators[index].symbol != op) {
        index++;
    }

    if (operators[index].symbol == "X") {
        cerr << "Operator not found\n";
        exit(1);
    }

    return index;
}

int get_op_ID(string op) {
    int index = get_op_index(op);

    return operators[index].order_of_p;
}

int get_precedence(string op) {
    int index = get_op_index(op);

    return operators[index].order_of_p/10;
}

// --------------------------------- variable address map and helpers

int next_slot = 0;
const int NUM_VARIABLES = 20;

string variables[NUM_VARIABLES] = {};

// offset in memory
const int base_address = 1000; 

void declare_var(string var) {

    if (next_slot == NUM_VARIABLES) {
        cerr << "Max variables reached\n";
        exit(1);
    }

    variables[next_slot] = var;
    next_slot++;
}

int get_var_index(string var) {
    int index;
    for (index =0 ; index < next_slot && variables[index] != var; index++) {
        // go until index found
    }

    return index == next_slot ? -1 : index;
}

int get_var_address(string var) {
    int index = get_var_index(var);

    if (index != -1) {
        return base_address + index;
    }
    else {
        cerr << "Error -- can't find address for the variable " << var << "\n";
        exit(1);
    }
}

int is_var_assigned(string var) {
    return get_var_index(var) != -1;
}

// ---------------------------------

class node {
    int digit;
    string value;
    node* left;
    node* right;
    string variable;

public:
    node(int digit, string val, node* left, node* right, string var) {
        this->digit = digit;
        this->value = val;
        this->left = left;
        this->right = right;
        this->variable = var;
    }

    node(int digit, string val, node* left, node* right) {
        this->digit = digit;
        this->value = val;
        this->left = left;
        this->right = right;
        this->variable = "";
    }

    node(string val, node* left, node* right) {
        this->digit = -1;
        this->value = val;
        this->left = left;
        this->right = right;
        this->variable = "";
    }

    node(string var) {
        this->digit = -1;
        this->value = "V";
        this->left = nullptr;
        this->right = nullptr;
        this->variable = var;
    }

    void print_equals() {
        if (this->left->get_symbol() != "V") {
            cerr << "Error -- can only assign to a variable\n";
            exit(1);
        }
        else {
            cout << "Push " << get_var_address(this->left->variable) << ";&" << this->left->variable << "\n";
            right->print_postfix();
            cout << "Assign\n";
        }
    }

    void print_postfix() {
        int op_index = get_op_index(this->value);
        string op_symbol  = operators[op_index].symbol;
        string op_print = operators[op_index].print_instructions;

        if (op_symbol == "=") { 
            print_equals();
            return;
        }
        else {
            if (this->left)
                left->print_postfix(); 

            if (this->right) 
                right->print_postfix();


            if (op_symbol == "N") {
                cout << op_print << this->digit << "\n";
            }
            else if (op_symbol == "V") {
                if (is_var_assigned(this->variable)) {
                    cout << "Push (" << get_var_address(this->variable) << ")\n";
                }
                else {
                    cout << "Push " << this->variable << "\n";
                }
            }
            else {
                cout << op_print;
            }
        }
    }

    string get_symbol() {
        return this->value;
    }

    int get_digit() {
        return this->digit;
    }

    void set_digit(int digit) {
        this->digit = digit;
    }

    string get_var() {
        if (this->value == "V") {
            return this->variable;
        }
        else {
            cerr << "Can't call get_var on a non-variable type\n";
            exit(1);
        }
    }
};

// ------------------------------- stack and stack helpers

stack<node*> digit_stack;

const int OP_STACK_SIZE = 10;
string operator_stack[OP_STACK_SIZE] = {};
int operator_index = 0; // points to the next available slot

string top_operator() {
    return operator_stack[operator_index-1];
}

void push_operator(string op) {
    operator_stack[operator_index++] = op;

    if (operator_index == OP_STACK_SIZE) {
        cerr << "Operator stack full.\n";
        exit(1);
    }
}

string pop_operator() {
    string top = top_operator();
    operator_stack[--operator_index] = '\0';
    return top;
}

// -------------------------------- reduction helpers

void reduce_negation() {
    node* num1 = digit_stack.pop();
    node* result = new node("@", num1, nullptr);
    digit_stack.push(result);
    (void)pop_operator();
}

void reduce_to_number(int num) {
    node* temp = new node(num, "N", nullptr, nullptr);
    digit_stack.push(temp);
}

void reduce_to_variable(string var) {
    node* temp = new node(var);
    digit_stack.push(temp);
}

void reduce() {
    node* num2 = digit_stack.pop();
    node* num1 = digit_stack.pop();
    node* op;
    string op_char = pop_operator();

    if (get_precedence(op_char) > 0) {
        op = new node(op_char, num1, num2);
    }
    else {
        cerr << "Reduce: Cannot reduce with type " << op_char << endl;
        exit(1);
    }

    digit_stack.push(op);
}

void reduce_until_boundary(string lowerbound, string error) {
    string top = top_operator();
    while (top != lowerbound && top != error) {
        if (top == "@") {
            reduce_negation();
        }
        else {
            reduce();
        }
        top = top_operator();
    }

    if (top == error) {
        cerr << "Syntax error\n";
        exit(1);
    }
}

// ---------------------------------------------------------------------- control flow evaluator

void control_flow_evaluate(string c_flow) {
    int index = 0;
}

// ---------------------------------------------------------------------- statement evaluator

void statement_evaluate(string exp) {
    int index = 0;
    char current = exp[index];

    cout << ">> " << exp << "\n";

    int q = 0;
    for (current = exp[index]; current != '\0'; current = exp[++index]) {

        if (current == ' ') {
            continue;
        }

        if (q == 0) {
            if (current == '(') {
                string current_str = get_string(current);
                push_operator(current_str);
                continue;
            }

            if (current == '-') {
                if (top_operator() == "@") {
                    (void)pop_operator();
                }
                else {
                    push_operator("@");
                }
                continue;
            }

            if (isdigit(current)) { // gobble nums
                int num = 0;
                while (isdigit(current)) { 
                    num = num * 10 + current - '0';
                    current = exp[++index];
                }

                if (isalpha(current) || current == '_') {
                    cerr << "Error -- improper variable format\n";
                    exit(1);
                }

                index--;
                reduce_to_number(num);
                q = 1;

                while (top_operator() == "@") { 
                    reduce_negation();
                }
            }
            else if (isalpha(current) || current == '_') { // gobble variables
                    string var;
                    while (isalnum(current) || current == '_') {
                            var += current;
                            current = exp[++index];
                    }
                    index--;
                    if (!is_var_assigned(var)) {
                        declare_var(var);
                    }
                    reduce_to_variable(var);
                    q = 1;
                    
                    while (top_operator() == "@") { 
                        reduce_negation();
                    }
                }
            else {
                cerr << "Can't read " << current << endl;
                exit(1);
            }
        }
        else {

            if (current == ')') {
                reduce_until_boundary("(", "$");

                (void)pop_operator();

                while (top_operator() == "@") {
                    reduce_negation();
                }

                continue;
            }

            string op_str = "";         
            while (ispunct(current) && (current != '(' && current != ')')) {
                op_str += get_string(current);
                current = exp[++index];
                
                if (current == '-') {
                    // pass negation off to state 0
                    break;
                }
            }
            current = exp[--index];

            string top = top_operator();
            int top_precedence = get_precedence(top);
            int current_precedence = get_precedence(op_str);

            if (top_precedence >= current_precedence) {                
                if (op_str == "=" && top == "=") {
                    // is right to left
                }
                else {
                    reduce();
                }
            }

            push_operator(op_str);
            q = 0;
        }
    }

    if (q == 1) {
        reduce_until_boundary("$", "(");

        node* expression_tree = digit_stack.top();
        expression_tree->print_postfix();
    }
    else {
        cerr << "Not a valid expression: " << exp << endl;
        exit(1);
    }
}

// -------------------------------------------------------------------------------------

int main(int argc, char **argv) {
    string line; 

    digit_stack.push(new node("$", nullptr, nullptr));
    push_operator("$");

    if (argc == 2) {
        line = argv[1];
        statement_evaluate(line);
    }
    else {
        cout << "Enter an expression:\n";
        while (getline(cin, line)) {
            statement_evaluate(line);
            (void)digit_stack.pop();
        }
    }

    return 0;
}