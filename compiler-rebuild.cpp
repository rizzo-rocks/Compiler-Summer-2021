#include <iostream>

using namespace std;

string get_string(char x)
{
    // string class has a constructor
    // that allows us to specify size of
    // string as first parameter and character
    // to be filled in given size as second
    // parameter.
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

// --------------------------------- precedence table and helper

/*struct op {               why won't you work??
    int order_of_p;
    //string symbol; don't need this bc it's in the node* ? not sure
    string print_instructions;
} neg, exp, mult, div, add, minus, lt, gt, ltet, gtet, net, et, _and, _or, assign, num, var, open_paren, closed_paren, end, not_found;

struct op neg; neg.order_of_p = 101;   neg.symbol = "@";
exp.order_of_p = 91;    exp.symbol = "^";
mult.order_of_p = 81;   mult.symbol = "*";
div.order_of_p = 82;    div.symbol = "/";
add.order_of_p = 71;    add.symbol = "+";*/

int order_of_ops[] = { 
    101, 91,  81,  82,  71,  72,  61,  62,   63,    64,   65,   66,  51,  41,  31,  21,  22,  11,  12,  0,  -1
};
string ops[] = {
    "@", "^", "*", "/", "+", "-", "<", ">", "<=", ">=", "!=", "==", "&", "|", "=", "N", "V", "(", ")", "$", "X"
};

/* Don't currently use
const int ORDER_OF_OPS_SIZE  = (sizeof(order_of_ops) / sizeof(order_of_ops[0]))/2;
*/

int get_op_index(string op) {
    int index = 0;
    while (ops[index] != op) {
        index++;
    }

    if (ops[index] == "X") {
        cerr << "Operator not found\n";
        exit(1);
    }

    return index;
}

int get_op_ID(string op) {
    int index = get_op_index(op);

    return order_of_ops[index];
}

int get_precedence(string op) {
        int index = get_op_index(op);

        return order_of_ops[index]/10;
}

// --------------------------------- variable address map and helpers

int next_slot = 0;
const int NUM_VARIABLES = 20;

string variables[NUM_VARIABLES] = {};
int addresses[NUM_VARIABLES] = {};
int assigned[NUM_VARIABLES] = {}; 

void add_address(string var) {
    int base_address = 1000;    // offset in memory

    if (next_slot == NUM_VARIABLES) {
        cerr << "Max variables reached\n";
        exit(1);
    }

    variables[next_slot] = var;
    addresses[next_slot] = base_address + next_slot;

    next_slot++;
}

int get_address_index(string var) {
    int index = 0;
    for (string current = variables[index]; current != var; current = variables[++index]) {
        // find the index matching var
    }

    if (variables[index] == var) {
        return index;
    }
    else { 
        return -1;
    }
}

int get_address(string var) {
    int index = get_address_index(var);

    if (index != -1) {
        return addresses[index];
    }
    else {
        cerr << "Error -- can't find address for the variable " << var << "\n";
        exit(1);
    }
}

/*int get_value_at_address(string var) {    // storing only whether it was assigned or not, not specific value
    int index = get_address_index(var);

    if (index != -1) {
        return values[index];
    }
    else {
        cerr << "Error -- can't find value for the variable " << var << "\n";
        exit(1);
    }
}*/

void assign_at_address(string var) {
    int index = get_address_index(var);

    if (index != -1) {
        assigned[index] = 1;
    }
    else {
        cerr << "Error -- could not find " << var << " in addressbook\n";
        exit(1);
    }
}

int is_assigned(string var) {
    int index = get_address_index(var);

    if (index == -1) {
        return 0;
    }
    else 
    {
        if (assigned[index]) {
            return 1;
        }
        return 0;
    }
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
        if (this->left->get_char() != "V") {
            cerr << "Error -- can only assign to a variable\n";
            exit(1);
        }
        else {
            cout << "Push " << get_address(this->left->variable) << ";&" << this->left->variable << "\n";
            right->print_postfix();
            cout << "Assign (" << get_address(this->left->variable) << ")\n";
        }
    }

    void print_postfix() {
        int op_id = get_op_ID(this->value);

        if (/*this->get_char()*/ op_id == 31) { 
            print_equals();
            return;
        }
        else {
            if (this->left)
                left->print_postfix(); 

            if (this->right) 
                right->print_postfix();

            //int op_id = get_op_ID(this->value);

            switch (op_id) {
                case 21:   cout << "Push " << this->digit << "\n"; break;
                case 22:  
                    if (is_assigned(this->variable)) {
                        cout << "Push (" << get_address(this->variable) << ")\n";
                    }
                    else {
                        cout << "Push " << this->variable << "\n"; break;
                    }
                    break;
                case 61:    cout << "LT\n";      break;
                case 62:    cout << "GT\n";      break;
                case 63:    cout << "LTET\n";    break;
                case 64:    cout << "GTET\n";    break;
                case 66:    cout << "ET\n";      break;
                case 65:    cout << "NET\n";     break;   
                case 71:    cout << "Sum\n";     break;
                case 72:    cout << "Minus\n";   break;
                case 81:    cout << "Mul\n";     break;
                case 82:    cout << "Div\n";     break;
                case 101:   cout << "Neg\n";     break;
                case 51:    cout << "And\n";     break;
                case 41:    cout << "Or\n";      break;
                case 91:    cout << "Exp\n";     break;
                default:  
                    cerr << "Unknown node " << this->value << "\n";  
                    exit(1);
            }
        }
    }

    string get_char() {
        return this->value;
    }

    /*void set_char(string c) {                             don't currently use
        this->value = c;
    }*/

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

    /*void peek_left() {                                    don't currently use
        cout << this->left->get_char() << " char\n";
        cout << this->left->get_digit() << " digit\n";
    }

    void peek_right() {
        cout << this->right->get_char() << " char\n";
        cout << this->right->get_digit() << " digit\n";
    }*/
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

/*void print_op_stack() {                               don't currently use
    for (int i = 0; i < OP_STACK_SIZE; i++) {
        cout << operator_stack[i] << '\n';
    }
}*/

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
                    reduce_to_variable(var);
                    q = 1;
                    
                    //if (!is_assigned(var)) { // throws a malloc_error but we need some way to check if the var is already in the addressbook so we don't duplicate
                        add_address(var);
                    //}
                    
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

            /*if (current == '<' || current == '>') {     // gobble when we encounter possible 2-char op's

            }*/
            /*int num = 0;
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
                q = 1; */
            /*string current_str = "";
            while (ispunct(current)) { // gobble operator
                current_str += current;
                current = exp[++index];
            }

            index--;   */ 
            string current_str = get_string(current);

            string top = top_operator();
            int top_precedence = get_precedence(top);
            int current_precedence = get_precedence(current_str);
            //int current_precedence = get_precedence(current);

            if (top_precedence >= current_precedence) {
                if (current == '=') {
                    string last_var = digit_stack.top()->get_var();
                    assign_at_address(last_var);
                }
                
                if (current == '=' && top == "=") {
                    // is right to left
                }
                else {
                    reduce();
                }
            }

            push_operator(current_str);
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