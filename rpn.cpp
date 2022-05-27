#include <iostream>
#include <stack>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <cmath>

using namespace std;

int gcd(int a, int b)
{
    if (b > a)
    {
        int t = a;
        a = b;
        b = t;
    }
    while (a && b)
    {
        int t = b;
        b = a % b;
        a = t;
    }
    return (a + b);
}

int lcm(int a, int b)
{
    return (a * b) / gcd(a, b);
}

int rpn(string str1)//const char *str)
{
    stack<int> numbers;
    int num = 0, n1, n2, res;
    char *end;
    const char* str = str1.c_str();
    for (int i = 0; i < (strlen(str)); ++i)
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            num = strtol(str + i, &end, 10);
            numbers.push(num);
            i = end - str;
        }
        else if (str[i] == ' ')
            continue;
        else
        {
            n2 = numbers.top();
            numbers.pop();
            n1 = numbers.top();
            numbers.pop();
            switch (str[i])
            {
            case '+':
                res = n1 + n2;
                break;
            case '-':
                res = n1 - n2;
                break;
            case '*':
                res = n1 * n2;
                break;
            case '/':
                res = n1 / n2;
                break;
            case '^':
                res = pow(n1, n2);
                break;
            case 'g':
                res = gcd(n1, n2);
                break;
            case 'l':
                res = lcm(n1, n2);
                break;
            default:
                cout << "error!";
                break;
            }
            numbers.push(res);
        }
    }
    return numbers.top();
}

string to_rpn(const char *str)
{
    string output;
    char *end;
    stack<char> operators;
    int prev_priority = 0, curr_priority;
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            int num = strtol(str + i, &end, 10);
            auto strnum = to_string(num);
            i = end - str;
            output += " " + strnum + " ";
        }

        else if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' )
        {
            switch (str[i])
            {
            case '+':
                curr_priority = 2;
                break;
            case '-':
                curr_priority = 2;
                break;
            case '*':
                curr_priority = 3;
                break;
            case '/':
                curr_priority = 3;
                break;
            default:
                cout << "error";
                break;
            }

            if ((operators.empty()) || (prev_priority < curr_priority))
                {
                    operators.push(str[i]);
                    prev_priority = curr_priority;
                }
                else if (prev_priority >= curr_priority)
                {

                    while (prev_priority >= curr_priority)
                    {
                        //cout << operators.top() << endl;;
                        output += operators.top();
                        //cout << output << endl;
                        operators.pop();
                        if (operators.empty())
                            prev_priority = 0;
                        else if(operators.top() == '+' || operators.top() == '-'){
                            prev_priority = 2;
                        }
                        else if(operators.top() == '*' || operators.top()=='/')
                            prev_priority == 3;
                        else if(operators.top()=='(') prev_priority = 1;
                    }
                    operators.push(str[i]);
                    prev_priority = curr_priority;
                    
                }
        }
        else if(str[i] == '(') {
            operators.push(str[i]);
        }
        else if(str[i]==')'){
            while(operators.top() != '('){
                output += operators.top();
                operators.pop();
            }
            if(operators.top() == '(') operators.pop();
        }
        
        else if(str[i] == 'g'){
            i+=4;
            int ind1 = string(str).find_first_of(',', i);
            const char* p1 = string(str).substr(i, ind1-i).c_str();
            i = ind1 + 1;

            int ind2 = string(str).find_first_of(')', i);
            const char* p2 = string(str).substr(i, ind2-i).c_str();

            output += " " + to_rpn(p1) + " " + to_rpn(p2) + " " + "g";
            i = ind2;
        }
        else if(str[i] == 'l'){
            i+=4;
            int ind1 = string(str).find_first_of(',', i);
            const char* p1 = string(str).substr(i, ind1-i).c_str();
            i = ind1 + 1;

            int ind2 = string(str).find_first_of(')', i);
            const char* p2 = string(str).substr(i, ind2-i).c_str();

            output += " " + to_rpn(p1) + " " + to_rpn(p2) + " " + "l";
            i = ind2;
        }
    }

    while (!operators.empty())
    {
        output += operators.top();
        operators.pop();
    }
    return output;
}

int main()
{ 
    string str1 = "10 25 l 12 - 8 +";
    int s1 = rpn(str1);
    //cout << s1 << endl;

    const char *str2 = "5 * 4 - 7 + lcm(3 + 1 * 7, 22 + 5) - 2"; //  5 4 * 7 - 3 1 7 * + 22 5 + l + 2 -
    // 3 1 7 * +    22 5 +    l
    string s2 = to_rpn(str2);
    
    cout << s2 << endl;
    cout << rpn(s2);
    return 0;
}