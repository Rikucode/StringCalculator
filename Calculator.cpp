#include <iostream>
#include <stack>
#include <cmath>
#include <iomanip>
#include <string>
#include <sstream>

using namespace std;

const double pi = acos(-1);

struct Lexema {
    char type;
    double value;
};

int getPrioritet(char cur_char){
    if (cur_char == '+' || cur_char == '-') {
        return 1;
    }
    if (cur_char == '*' || cur_char == '/' || cur_char == '%') {
        return 2;
    }
    if (cur_char == '^') {
        return 3;
    }
    if (cur_char == '!') {
        return 4;
    }
    return 0;
}

bool maths(stack <Lexema> & stack_number, stack <Lexema> & stack_operation) {
    double foo, buz, bar;
    Lexema lexema;
    foo = stack_number.top().value;
    stack_number.pop();
    switch(stack_operation.top().type){
        case '+' :
            buz = stack_number.top().value;
            stack_number.pop();
            bar = foo + buz;
            lexema.value = bar;
            lexema.type = '0';
            stack_number.push(lexema);
            break;

        case '-' :
            buz = stack_number.top().value;
            stack_number.pop();
            bar = buz - foo;
            lexema.value = bar;
            lexema.type = '0';
            stack_number.push(lexema);
            break;

        case '*' :
            buz = stack_number.top().value;
            stack_number.pop();
            bar = foo * buz;
            lexema.value = bar;
            lexema.type = '0';
            stack_number.push(lexema);
            break;

        case '/' :
            buz = stack_number.top().value;
            stack_number.pop();
            if (foo == 0) {
                cerr << "Error: divided by zero!";
                return false;
            }
            bar = buz / foo;
            lexema.value = bar;
            lexema.type = '0';
            stack_number.push(lexema);
            break;

        case '%' :
            buz = stack_number.top().value;
            stack_number.pop();
            if (foo == 0) {
                cerr << "Error: Divided by zero!";
                return false;
            }
            bar = buz - (floor(buz / foo) * foo);
            lexema.value = bar;
            lexema.type = '0';
            stack_number.push(lexema);
            break;

        case '^' :
            buz = stack_number.top().value;
            stack_number.pop();
            bar = pow(buz, foo);
            lexema.value = bar;
            lexema.type = '0';
            stack_number.push(lexema);
            break;

        case '!' :
            bar = tgamma(foo+1);
            if (isnan(bar) || isinf(bar)){
                cerr << "Error: Factorial is undefined!";
                return false;
            }
            lexema.value = bar;
            lexema.type = '0';
            stack_number.push(lexema);
            break;

        default :
            cerr << "Error: Unknown operation or incorrect input!";
            return false;
    }
    stack_operation.pop();
    return true;
}

int calculate(string str){
    stringstream sstr(str);
    char cur_char;
    bool first_minus_flag = true;
    stack <Lexema> stack_number;
    stack <Lexema> stack_operation;
    Lexema lexema;
    while(true) {
        cur_char = sstr.peek();
        if (cur_char == '\377') break;
        if (cur_char == ' ') {
            sstr.ignore();
            continue;
        }
        if (cur_char >= '0' && cur_char <= '9' || first_minus_flag && cur_char == '-') {
            first_minus_flag = false;
            lexema.type = '0';
            sstr >> lexema.value;
            stack_number.push(lexema);
            continue;
        }
        if (cur_char == '+' || cur_char == '-' || cur_char == '*' || cur_char == '/' || cur_char == '%' || cur_char == '^' || cur_char == '!') {
            if (stack_operation.empty() || getPrioritet(cur_char) > getPrioritet(stack_operation.top().type)) {
                lexema.type = cur_char;
                lexema.value = 0;
                stack_operation.push(lexema);
                sstr.ignore();
                continue;
            }
            else {
                if (maths(stack_number, stack_operation)) {
                    stack_number.top().value;
                    continue;
                }
            }
        }
        if (cur_char == '(') {
            lexema.type = cur_char;
            lexema.value = 0;
            stack_operation.push(lexema);
            sstr.ignore();
            first_minus_flag = true;
            continue;
        }
        if (cur_char == ')') {
            while (stack_operation.top().type != '(') {
                if (!maths(stack_number, stack_operation)) {
                    return -1;
                }
            }
            stack_operation.pop();
            sstr.ignore();
            continue;
        }
        else {
            cerr << "Error: Incorrect input!";
            return -1;
        }
    }
    while (!stack_operation.empty()) {
        if (!maths(stack_number, stack_operation)) {
            return -1;
        }
    }
    cout << setprecision(10) << "Answer is " << stack_number.top().value << endl;
    return 0;
}

int main() {
    cout << "String calculator." << endl << "If you wanna quit leave the line empty";
    string str;
    while (true) {
    getline(cin, str);
    if (str == "") return 0;
    calculate(str);
    }
    return 0;
}