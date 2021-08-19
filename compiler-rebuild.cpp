#include <iostream>

using namespace std;

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

int order_of_ops[] = {
    '@', 91,
    '^', 81,
    '*', 71,
    '/', 72,
    '+', 61,
    '-', 62,
    '&', 51,
    '|', 41,
    '=', 31,
    'N', 21,
    'V', 22,
    '(', 11,
    ')', 12,
    '$', 0,
    'X', -1
};

/* Don't currently use
const int ORDER_OF_OPS_SIZE  = (sizeof(order_of_ops) / sizeof(order_of_ops[0]))/2;
*/

int get_precedence(char op) {
        int index = 0;
        while ((order_of_ops[index] != op) 
            && (order_of_ops[index] != '\0')) {
            index += 2;
        }

        if (order_of_ops[index] == 'X') {
            cerr << "Operator not found\n";
            exit(1);
        }

        return order_of_ops[++index]/10;
}

// --------------------------------- variable address map and helpers

int next_slot = 0;
string variables[] = {};
int addresses[] = {};
int values[] = {};

void add_address(string var, int val) {
    int base_address = 1000;    // offset in memory

    variables[next_slot] = var;
    addresses[next_slot] = base_address + next_slot;
    values[next_slot] = val;

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
        return -1;
    }
}

int get_value(string var) {
    int index = get_address_index(var);

    if (index != -1) {
        return values[index];
    }
    else {
        return -1;
    }
    
}

// ---------------------------------

class node {
    int digit;
    char value;
    node* left;
    node* right;
    string variable;

public:
    node(int digit, char val, node* left, node* right, string var) {
        this->digit = digit;
        this->value = val;
        this->left = left;
        this->right = right;
        this->variable = var;
    }

    node(int digit, char val, node* left, node* right) {
        this->digit = digit;
        this->value = val;
        this->left = left;
        this->right = right;
        this->variable = "";
    }

    node(char val, node* left, node* right) {
        this->digit = -1;
        this->value = val;
        this->left = left;
        this->right = right;
        this->variable = "";
    }

    node(string var) {
        this->digit = -1;
        this->value = 'V';
        this->left = nullptr;
        this->right = nullptr;
        this->variable = var;
    }

    void print_equals() {
        if (this->left->get_char() != 'V') {
            cerr << "Error -- can only assign to a variable\n";
            exit(1);
        }
        else {
            cout << "Push &" << this->left->variable << "\n";
            right->print_postfix();
            cout << "Assign\n";
        }
    }

    void print_postfix() {
        if (this->value == '=') { 
            print_equals();
            return;
        }
        else {
            if (this->left)
                left->print_postfix(); 

            if (this->right) 
                right->print_postfix();

            switch (this->value) {
                case 'N':  cout << "Push " << this->digit << "\n"; break;
                case 'V':  cout << "Push " << this->variable << "\n"; break;
                case '+':  cout << "Sum\n";    break;
                case '-':  cout << "Minus\n";  break;
                case '*':  cout << "Mul\n";    break;
                case '/':  cout << "Div\n";    break;
                case '@':  cout << "Neg\n";    break;
                case '&':  cout << "And\n";    break;
                case '|':  cout << "Or\n";     break;
                case '^':  cout << "Exp\n";    break;
                default:  
                    cerr << "Unknown node " << this->value << "\n";  
                    exit(1);
            }
        }
    }

    char get_char() {
        return this->value;
    }

    void set_char(char c) {
        this->value = c;
    }

    int get_digit() {
        return this->digit;
    }

    void set_digit(int digit) {
        this->digit = digit;
    }

    string get_var() {
        if (this->value == 'V') {
            return this->variable;
        }
        else {
            cerr << "Can't call get_var on a non-variable type\n";
            exit(1);
        }
    }

    void peek_left() {
        cout << this->left->get_char() << " char\n";
        cout << this->left->get_digit() << " digit\n";
    }

    void peek_right() {
        cout << this->right->get_char() << " char\n";
        cout << this->right->get_digit() << " digit\n";
    }
};

// ------------------------------- stack and stack helpers

stack<node*> digit_stack;

const int OP_STACK_SIZE = 10;
char operator_stack[OP_STACK_SIZE] = {};
int operator_index = 0; // points to the next available slot

char top_operator() {
    return operator_stack[operator_index-1];
}

void push_operator(char op) {
    operator_stack[operator_index++] = op;

    if (operator_index == OP_STACK_SIZE) {
        cerr << "Operator stack full.\n";
        exit(1);
    }
}

char pop_operator() {
    char top = top_operator();
    operator_stack[--operator_index] = '\0';
    return top;
}

void print_op_stack() {
    for (int i = 0; i < OP_STACK_SIZE; i++) {
        cout << operator_stack[i] << '\n';
    }
}

// -------------------------------- reduction helpers

void reduce_negation() {
    node* num1 = digit_stack.pop();
    node* result = new node('@', num1, nullptr);
    digit_stack.push(result);
    (void)pop_operator();
}

void reduce_to_number(int num) {
    node* temp = new node(num, 'N', nullptr, nullptr);
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
    char op_char = pop_operator();

    if (get_precedence(op_char) > 0) {

        /*if (op_char == '=') {
            if (get_address_index(num1->get_var()) == -1) {
                add_address(num1->get_var(), num2->get_digit());
            }
            else {
                // overwrite what's there?
            }
        }*/

        op = new node(op_char, num1, num2);
    }
    else {
        cerr << "Reduce: Cannot reduce with type " << op_char << endl;
        exit(1);
    }

    digit_stack.push(op);
}

void reduce_until_boundary(char lowerbound, char error) {
    char top = top_operator();
    while (top != lowerbound && top != error) {
        if (top == '@') {
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

// ---------------------------------------------------------------------- interpretation 

void evaluate(string exp) {
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
                push_operator(current);
                continue;
            }

            if (current == '-') {
                if (top_operator() == '@') {
                    (void)pop_operator();
                }
                else {
                    push_operator('@');
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

                while (top_operator() == '@') { 
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
                    
                    while (top_operator() == '@') { 
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
                reduce_until_boundary('(', '$');

                (void) pop_operator();

                while (top_operator() == '@') {
                    reduce_negation();
                }

                continue;
            }

            char top = top_operator();
            int top_precedence = get_precedence(top);
            int current_precedence = get_precedence(current);

            if (top_precedence >= current_precedence) {
                if (current == '=' && top == '=') {
                    // is right to left
                }
                else {
                    reduce();
                }
            }

            push_operator(current);
            q = 0;
        }
    }

    if (q == 1) {
        reduce_until_boundary('$', '(');

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

    digit_stack.push(new node('$', nullptr, nullptr));
    push_operator('$');

    if (argc == 2) {
        line = argv[1];
        evaluate(line);
    }
    else {
        cout << "Enter an expression:\n";
        while (getline(cin, line)) {
            evaluate(line);
            (void)digit_stack.pop();
        }
    }

    return 0;
}