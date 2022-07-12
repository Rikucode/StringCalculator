#include <iostream>
#include <stack>
#include <cmath>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

const double pi = acos(-1);

struct Lexema {
    char type;
    double value;
};

string StringReplacer(const string& inputStr, const string& src, const string& dst)
{
    string result(inputStr);

    size_t pos = result.find(src);
    while(pos != string::npos) {
        result.replace(pos, src.size(), dst);
        pos = result.find(src, pos);
    }

    return result;
}

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
    bool first_symbol = true;
    bool previous_sign = false;
    bool num_after_sk_flag = false;
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
        if (cur_char >= '0' && cur_char <= '9') {
            if (num_after_sk_flag){
                cerr << "Error: incorrect input" << endl;
                return -1;
            }
            lexema.type = '0';
            sstr >> lexema.value;
            previous_sign = false;
            stack_number.push(lexema);
            first_symbol = false;
            continue;
        }
        if ((cur_char == '+' || cur_char == '-' || cur_char == '*' || cur_char == '/' || cur_char == '%' || cur_char == '^' || cur_char == '!') && !first_symbol) {
            if (previous_sign) {
                cerr << "Error: Incorrect Input" << endl;
                previous_sign = false;
                return  -1;
            }
            if (stack_operation.empty() || getPrioritet(cur_char) > getPrioritet(stack_operation.top().type)) {
                num_after_sk_flag = false;
                lexema.type = cur_char;
                previous_sign = true;
                first_symbol = false;
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
            previous_sign = false;
            first_symbol = false;
            num_after_sk_flag = false;
            lexema.type = cur_char;
            lexema.value = 0;
            stack_operation.push(lexema);
            sstr.ignore();
            continue;
        }
        if (cur_char == ')') {
            first_symbol = false;
            previous_sign = false;
            num_after_sk_flag = true;
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
            cerr << "Error: Incorrect input!" << endl;
            return -1;
        }
    }
    while (!stack_operation.empty()) {
        if (!maths(stack_number, stack_operation)) {
            cerr << "Error: incorrect input!" << endl;
            return -1;
        }
    }
    cout << setprecision(10) << "Answer is " << stack_number.top().value << endl;
    return 0;
}

int main() {
    cout << "String calculator." << endl << "If you wanna quit leave the line empty" << endl;
    string str;
    int count_l, count_r;
    int exitcode;
    while (true) {
        getline(cin, str);
        count_l = count(str.begin(),str.end(),'(');
        count_r = count(str.begin(),str.end(),')');
        if(count_r != count_l){
            cerr<<"Error: Incorrect Input!"<<endl;
            continue;
        }
        str = StringReplacer(str, "(-", "(0-");
        str = StringReplacer(str, "()", "0");
//        str = StringReplacer(str, "--", "");
//        str = StringReplacer(str, "-+", "-");
//        str = StringReplacer(str, "+-", "-");
        if (str[0] == '-') {
            str = '0' + str;
        }
        if (str == "") return 0;
        exitcode = calculate(str);
    }
    return 0;
}