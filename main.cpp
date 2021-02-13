#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <sstream>
#include <fstream>
#include "ArgumentManager.h"
#include "LinkedList.h"

using std::string;  using std::stringstream;
using std::stack;   using std::vector;

vector<char> getParentheses(const string& exp);
bool isBalanced(const vector<char>& exp);
bool isParenthesis(const char& c);
bool containsParentheses(const string& exp);
stack<string> getExpressionStack(const string& exp);
int calculateStack(stack<string>& S);

enum Twin {SARAH, ALEX};

int main(int argc, char* argv[]) {
    ArgumentManager am(argc, argv);
    std::ifstream ifs(am.get("input"));
    std::ofstream ofs(am.get("output"));

    LinkedList<int>* list_sarah = new LinkedList<int>();
    LinkedList<int>* list_alex = new LinkedList<int>();
    LinkedList<char>* actual_code = new LinkedList<char>();

    vector<int> vec_negatives;
    int num_invalids = 0;
    Twin twin;
    string line;
    while (getline(ifs, line)) {
        if (line.length() <= 0)
            continue;
        else if (line == "Sarah") {
            twin = SARAH;
            continue;
        }
        else if (line == "Alex") {
            twin = ALEX;
            continue;
        }
        else if (line.find("Passcode:") != string::npos) {
            stringstream ss(line.substr(9));
            char digit;
            while (ss >> digit)
                actual_code->add(digit);
            continue;
        }

        if (!isBalanced(getParentheses(line)))  {
            num_invalids++;
            continue;
        }

        stack<string> exp_stack = getExpressionStack(line);
        int res = calculateStack(exp_stack);
        bool foundNegative = false;
        for (vector<int>::iterator it = vec_negatives.begin(); !foundNegative && it != vec_negatives.end(); it++) {
            if (*it == -res) {
                vec_negatives.erase(it);
                foundNegative = true;
            }
        }
        if (foundNegative) 
            continue;
        if (res < 0) {
            if (!list_sarah->removeElement(-res) && !list_alex->removeElement(-res))
                vec_negatives.push_back(res);
        }
        else {
            if (twin == SARAH)
                list_sarah->add(res);
            else if (twin == ALEX)
                list_alex->add(res);
        }
    }

    ofs << "Sarah: ";
    list_sarah->write(ofs); 
    ofs << "\n";
    ofs << "Alex: ";
    list_alex->write(ofs);
    ofs << "\n";

    LinkedList<int>* list_code = LinkedList<int>::merge(list_sarah, list_alex);
    if (num_invalids > 0)
        list_code->removeIndex(num_invalids);
    list_code->reverse();
    ofs << "Decoded passcode: ";
    list_code->writeCode(ofs);
    ofs << "\n";
    ofs << "Actual passcode: ";
    actual_code->writeCode(ofs);

    ofs.flush();
    ofs.close();
    ifs.close();

    delete list_sarah;
    delete list_alex;
    delete actual_code;
    delete list_code;

    return 0;
}

stack<string> getExpressionStack(const string& exp) {
    vector<string> output;
    stack<char> operators;
    string operand = "";
    stringstream ss;
    ss.str(exp);
    char c;
    while (ss >> c) {
        if (c == '\0') continue;
        if (c == '+' || c == '-') {
            if (operand.length()) {
                output.push_back(operand);
                operand = "";
            }
            if (!operators.empty() && (operators.top() == '+' || operators.top() == '-')) {
                output.push_back(string(1, operators.top()));
                operators.pop();
            }
            operators.push(c);
        }
        else if (c == '(' || c == '{' || c == '[') {
            operators.push(c);
        }
        else if (c == ')') {
            if (operand.length()) {
                output.push_back(operand);
                operand = "";
            }
            while (operators.top() != '(') {
                output.push_back(string(1, operators.top()));
                operators.pop();
            }
            operators.pop();
        }
        else if (c == '}') {
            if (operand.length()) {
                output.push_back(operand);
                operand = "";
            }
            while (operators.top() != '{') {
                output.push_back(string(1, operators.top()));
                operators.pop();
            }
            operators.pop();
        }
        else if (c == ']') {
            if (operand.length()) {
                output.push_back(operand);
                operand = "";
            }
            while (operators.top() != '[') {
                output.push_back(string(1, operators.top()));
                operators.pop();
            }
            operators.pop();
        }
        else {
            operand.push_back(c);
        }
    }
    if (operand.length())
        output.push_back(operand);
    while (!operators.empty()) {
        output.push_back(string(1, operators.top()));
        operators.pop();
    }
    
    stack<string> res;
    while (!output.empty()) {
        res.push(output.back());
        output.pop_back();
    }
    return res;
}

int calculateStack(stack<string>& S) {
    stack<int> temp;
    while (!S.empty()) {
        string token = S.top();
        S.pop();
        if (token == "+") {
            int a = temp.top(); temp.pop();
            int b = temp.top(); temp.pop();
            temp.push(a + b);
        }
        else if (token == "-") {
            int a = temp.top(); temp.pop();
            int b = temp.top(); temp.pop();
            temp.push(b - a);
        }
        else {
            temp.push(std::stoi(token));
        }
    }
    return temp.top();
}

vector<char> getParentheses(const string& exp) {
    vector<char> tokens;
    for (size_t i = 0; i < exp.length(); i++) {
        char c = exp[i];
        if (isParenthesis(c))
            tokens.push_back(c);
    }
    return tokens;
}

bool containsParentheses(const string& exp) {
    return (exp.find('(') != string::npos ||
        exp.find('{') != string::npos ||
        exp.find('[') != string::npos);
}

bool isParenthesis(const char& c) {
    return (c == '(' || c == ')' ||
            c == '{' || c == '}' ||
            c == '[' || c == ']');
} 

bool isBalanced(const vector<char>& tokens) {
    stack<char> S;
    for (vector<char>::const_iterator it = tokens.begin(); it != tokens.end(); it++) {
        if (*it == '(' || *it == '{' || *it == '[') {
            S.push(*it);
        }
        else {
            if (S.empty()) return false;
            if (*it == ')' && S.top() != '(' ||
                *it == '}' && S.top() != '{' ||
                *it == ']' && S.top() != '[') 
                return false;
            else
                S.pop();
        }
    }
    if (S.empty()) return true;
    else return false;
}