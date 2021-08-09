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

// --------------------------------- converting op precedence array to struct

/*struct operators {
    char* output;
    int precedence;
    char op;
}; 

operators open_paren, closed_paren, negation, exp, mult, div, add, sub, _and, _or, num, end, not_found;

open_paren.output = */

// ---------------------------------

const int ORDER_OF_OPS_SIZE = 30;
int order_of_ops[ORDER_OF_OPS_SIZE] = {
    '=', 91,
    '@', 81,
    '^', 71,
    '*', 61,
    '/', 62,
    '+', 51,
    '-', 52,
    '&', 41,
    '|', 31,
    'N', 21,
    'V', 22,
    '(', 11,
    ')', 12,
    '$', 0,
    'X', -1
};

// -------------------------------- precedence helper

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

    void print_postfix() {
        if (this->left)
            left->print_postfix(); 

        if (this->right) 
            right->print_postfix();

        switch (this->value) {
            case 'N':  cout << "Push " << this->digit << "\n"; break;
            case 'V':  cout << "Push " << this->variable << "\n"; break;
            case '=':  cout << "Assign\n"; break; 
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

    char get_char() {
        return this->value;
    }

    int get_digit() {
        return this->digit;
    }

    void set_digit(int digit) {
        this->digit = digit;
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
        op = new node(op_char, num1, num2);
    }
    else {
        cerr << "Reduce: Cannot reduce with type " << op_char << endl;
        exit(1);
    }

    digit_stack.push(op);
}

// -------------------------------------------------------------------------------------

int main(int argc, char **argv) {
    int index = 0;
    string expression = "";

    if (argc == 2) {
        expression = argv[1];
    }
    else {
        std::cout << "Enter an expression:\n";
        cin >> expression;
    }

    char current = expression[index];

    digit_stack.push(new node('$', nullptr, nullptr));    
    push_operator('$');

    cout << ">> " << expression << "\n";

    int q = 0;
    for (current = expression[index]; current != '\0'; current = expression[++index]) {
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
                    current = expression[++index];
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
                            current = expression[++index];
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
                while (top_operator() != '(' && top_operator() != '$') {
                    reduce();
                }

                if (top_operator() == '(') (void) pop_operator();
                else if (top_operator() == '$') {
                    cerr << "Error -- Paren never closed\n";
                    exit(1);
                }

                while (top_operator() == '@') {
                    reduce_negation();
                }

                continue;
            }

            char top = top_operator();
            int top_precedence = get_precedence(top);
            int current_precedence = get_precedence(current);

            if (top_precedence >= current_precedence) {
                reduce();
            }

            push_operator(current);
            q = 0;
        }
    }

    if (q == 1) {
        while (top_operator() != '$') {
            char top = top_operator();
            if (top == '(') {
                cerr << "Error -- unclosed paren\n";
                exit(1);
            }
            else if (top == '@') {
                reduce_negation();
            }
            else {
                reduce();
            }
        }
        node* expression_tree = digit_stack.pop();
        expression_tree->print_postfix();
    }
    else {
        cerr << "Not a valid expression: " << expression << endl;
        exit(1);
    }

    return 0;
}