#include <iostream>
#include <fstream>

using std::cout;
using std::cin;
using std::string;
using std::cerr;
using std::fstream;

string get_string(char x)
{
    string s(1, x);
    return s;  
}

int pow(int num1, int num2) {
    int result = 1;

    for (int i=0; i<num2; i++) {
        result *= num1;
    }

    return result;
}

// ------------------------------------------------------------------------------ classes

template <typename S, typename T>
class map {
    S type1[100] = {};
    T type2[100] = {};
    int size = 0;

public:
    bool is_in(S element) {
        for (int i=0; i<size; i++) {
            if (type1[i] == element) {
                return true;
            }
        }

        return false;
    }

    T at(S element) {
        for (int i=0; i<size; i++) {
            if (type1[i] == element) {
                return type2[i];
            }
        }

        cerr << "Element not found in map\n";
        exit(1); 
    }

    void insert(S element1, T element2) {
        if (size == 100) {
            cerr << "Map is full\n";
            exit(1);
        }

        type1[size] = element1;
        type2[size] = element2;
        size++;
    }
    void insert(S element1) {
        if (size == 100) {
            cerr << "Map is full\n";
            exit(1);
        }

        type1[size] = element1;
        type2[size] = 0;
        size++;
    }

    void insert_value(S element1, T element2) {
        for (int i=0; i<size; i++) {
            if (type1[i] == element1) {
                type2[i] = element2;
                return;
            }
        }

        insert(element1, element2);
    }

    int map_size() {
        return size;
    }

    void print_map() {
        for (int i=0; i<size; i++) {
            cout << type1[i] << "\t-->\t" << type2[i] << "\n";
        }
    }
};

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
            cerr << "Stack is empty\n";
            exit(1);
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

// ------------------------------------------------------------------

int main(int argc, char **argv) {
    string line; 
    int line_index;

    /*if (argc == 2) {
        line = argv[1];
        //statement_evaluate(line);
    }
    else {*/
        cout << "\n";
        cout << "========== Interpreter ==========\n";

        stack<int> interpreter_stack;

        map<string, int> predefined_words;
        predefined_words.insert("Pop", 1);
        predefined_words.insert("Neg", 2);
        predefined_words.insert("Exp", 3);
        predefined_words.insert("Mul", 4);
        predefined_words.insert("Div", 5);
        predefined_words.insert("Sum", 6);
        predefined_words.insert("Minus", 7);
        predefined_words.insert("LT", 8);
        predefined_words.insert("GT", 9);
        predefined_words.insert("LE", 10);
        predefined_words.insert("GE", 11);
        predefined_words.insert("NE", 12);
        predefined_words.insert("EQ", 13);
        predefined_words.insert("And", 14);
        predefined_words.insert("Or", 15);
        predefined_words.insert("Assign", 16);
        predefined_words.insert("JMP", 17); 
        predefined_words.insert("BRZ", 18);
        predefined_words.insert("Push", 19);

        string lines[100] = {};

        line = "";
        int iline = 0;

        for (;;) {
            top3:
            if (!getline(cin, line)) break;
            if (line == "========== Interpreter ==========") break;

            if (line == "========== Compiler =========="
                || line == "Enter an expression:" 
                || ispunct(line[0])) {
                goto top3;
            }
            else {
                lines[iline++] = line; 
            }
        }

        int nlines = iline;

        int line_no = 0;
        line = "";

        map<string, int> labels;
        for (int i = 0;i < nlines;i++) {
            line = lines[i];
        
            int ich = 0;
            string word = "";
            char c = line[ich];
            if (c == '\0') continue;

            const char *p = strchr(line.c_str(), ':');
            if (!p) continue;

            while (c != ':') {
                word += get_string(c);
                c = line[++ich];
            }

            labels.insert(word, i);
        }

        map<int,int> var_values;

        int i=0;
        for (int i=0; i<nlines; i++) {
            if (lines[i] == "") break;

            string this_line = lines[i];
            cout << this_line << "\n";

            if (predefined_words.is_in(this_line)) {    // if the line matches exactly, easy
                int action = predefined_words.at(this_line);
                int num1 = 0;
                int num2 = 0;
                int result = 0;

                switch (action) {
                    case 1: // Pop
                        (void) interpreter_stack.pop();
                    break;
                    case 2: // Neg
                        num2 = interpreter_stack.pop();
                        interpreter_stack.push(num2*-1);
                    break;
                    case 3: //Exp
                        num2 = interpreter_stack.pop(); 
                        num1 = interpreter_stack.pop();
                        interpreter_stack.push(pow(num1, num2));
                    break;
                    case 4: // Mul
                        num2 = interpreter_stack.pop(); 
                        num1 = interpreter_stack.pop();
                        interpreter_stack.push(num1*num2);
                    break;
                    case 5: // Div
                        num2 = interpreter_stack.pop(); 
                        num1 = interpreter_stack.pop();
                        interpreter_stack.push(num1/num2);
                    break;
                    case 6: // Sum
                        num2 = interpreter_stack.pop(); 
                        num1 = interpreter_stack.pop();
                        interpreter_stack.push(num1+num2);
                    break;
                    case 7: // Minus
                        num2 = interpreter_stack.pop(); 
                        num1 = interpreter_stack.pop();
                        interpreter_stack.push(num1-num2);
                    break;
                    case 8: // LT
                        num2 = interpreter_stack.pop(); 
                        num1 = interpreter_stack.pop();
                        interpreter_stack.push(num1<num2);
                    break;
                    case 9: // GT
                        num2 = interpreter_stack.pop(); 
                        num1 = interpreter_stack.pop();
                        interpreter_stack.push(num1>num2);
                    break;
                    case 10: // LE
                        num2 = interpreter_stack.pop(); 
                        num1 = interpreter_stack.pop();
                        interpreter_stack.push(num1<=num2);
                    break;
                    case 11: // GE 
                        num2 = interpreter_stack.pop(); 
                        num1 = interpreter_stack.pop();
                        interpreter_stack.push(num1>=num2);
                    break;
                    case 12: // NE
                        num2 = interpreter_stack.pop(); 
                        num1 = interpreter_stack.pop();
                        interpreter_stack.push(num1!=num2);
                    break;
                    case 13: // EQ
                        num2 = interpreter_stack.pop(); 
                        num1 = interpreter_stack.pop();
                        interpreter_stack.push(num1==num2);
                    break;
                    case 14: // And
                    num2 = interpreter_stack.pop(); 
                        num1 = interpreter_stack.pop();
                        interpreter_stack.push(num1&&num2);
                    break;
                    case 15: // Or
                    num2 = interpreter_stack.pop(); 
                        num1 = interpreter_stack.pop();
                        interpreter_stack.push(num1||num2);
                    break;
                    case 16: // Assign
                        num2 = interpreter_stack.pop(); 
                        num1 = interpreter_stack.pop();

                        var_values.insert_value(num1, num2);
                        interpreter_stack.push(num2);
                    break;
                }
            }
            else // it's Push # or Push (#) or BRZ/JMP or a label 
            {
                int line_indexer = 0;
                char c = this_line[line_indexer];
                string part1 = "";
                string part2 = "";

                while (!isspace(c)) {
                    part1 += get_string(c);
                    c = this_line[++line_indexer];
                }
                while (isspace(c)) {
                    c = this_line[++line_indexer];
                }
                while (!isspace(c) &&c != '\0') {
                    part2 += get_string(c);
                    c = this_line[++line_indexer];
                }

                if (predefined_words.is_in(part1)) {
                    int action = predefined_words.at(part1);

                    switch (action) {
                        case 18: // BRZ
                        {
                            int top = interpreter_stack.pop();
                            if (top)
                                continue;
                        } // FALL THROUGH TO JMP
                        case 17: // JMP
                            if (labels.is_in(part2)) {
                                i = labels.at(part2);
                                continue;
                            }
                            else 
                            {
                                cerr << "This error shouldn't get thrown. Something has gone terribly wrong.\n";
                                exit(1);
                            }
                        break;
                        case 19: // Push
                            int digit_index = 0;
                            char next_digit = part2[digit_index];
                            int num_to_push = 0;

                            int addr;
                            if (part2[0] == '(') {
                                // indirect
                                addr = atoi(part2.c_str()+1);
                                num_to_push = var_values.at(addr);
                                interpreter_stack.push(num_to_push);
                            }
                            else {
                                num_to_push = atoi(part2.c_str());
                                interpreter_stack.push(num_to_push);
                            }
                        break;
                    }
                }
                else {
                    // it's a label
                }
            }
        }

        cout << "\n========== Results ==========\n";
        for (int addr = 1000; addr <= 1005; addr++) {
            if (var_values.is_in(addr)) {
                cout << addr << " " << var_values.at(addr) << "\n";
            }
        }
    //}    

    return 0;
}