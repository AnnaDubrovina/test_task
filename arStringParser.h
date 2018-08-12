//
//  adStringParser.h
//  calculator
//
//  Created by Анна Дубровина on 11.08.2018.
//  Copyright © 2018 Анна Дубровина. All rights reserved.
//

#ifndef adStringParser_h
#define adStringParser_h

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <stack>
#include <regex>
#include <unordered_map>
#include "float.h"

using namespace std;

vector<char> numSymbols = {'0','1','2','3','4','5','6','7','8','9','.',','};
std::vector<char> operationOnly = {'+','-','*','/'};


bool checkString(string &str)
{
    //Для начала уберем все пробелы.
    string::iterator end_pos = std::remove(str.begin(), str.end(), ' ');
    str.erase(end_pos, str.end());
    
    //теперь проверим на наличие посторонних символов
    //todo
    regex rx("[^0123456789\\.\\,\\+/\\-\\*\\(\\)]+");
    std::smatch m;
    std::regex_search(str, m, rx);
    if(m.size() !=0)
    {
        cout<<"wrong simbol:"<<m[0]<<endl;
        return false;
    }
    
    //приведем к единому виду точки и запятые
    replace( str.begin(), str.end(), ',', '.');
    
    //подсчитаем кол-во скобок
    int counter =0;
    for(char c: str)
    {
        if(c == '(')
            counter++;
        else if(c == ')')
            counter--;
    }
    
    if(counter < 0)
    {
        cout<<"wrong amount of ')'"<<endl;
        return false;
    }
    if(counter > 0)
    {
        cout<<"wrong amount of '('"<<endl;
        return false;
    }
    
    //проверим, не идут ли подряд арифметические операции (+- и -- - исключения)
    regex rx2("([\\+\\-\\*/]){2,}");
    std::regex_search(str, m, rx2);
    if(m.size() !=0)
    {
        string buf = m[0];
        if(!(buf.size() == 2 && buf[1]=='-'))
        {
            cout<<"wrong simbol:"<<m[0]<<endl;
            return false;
        }
    }
    
    return true;
    
}
//для избавления от вложенных скобок используем обратную польскую запись
string toReversePolska(const string expression)
{
    stack<char> s;
    
    string result;
    string curNumber;
    
    unordered_map<char, int> opPriority;
    opPriority.insert(std::make_pair('*',3));
    opPriority.insert(std::make_pair('/',3));
    opPriority.insert(std::make_pair('+',2));
    opPriority.insert(std::make_pair('-',2));
    opPriority.insert(std::make_pair('(',1));

    bool prevOper = true;//для поимки отрицательного числа. Если начало строки или после символа операции, то это отрицательное число.
    for(int i =0;i < expression.size();i++)
    {
        char c = expression[i];
        //считаем все символы числа
        if(find(numSymbols.begin(), numSymbols.end(), c) != numSymbols.end())
        {
            curNumber+=c;
            prevOper = false;
            continue;
        }
        else if(find(operationOnly.begin(), operationOnly.end(), c) != operationOnly.end())//если знак операции
        {
            if(c == '-' && prevOper)
            {
                curNumber = 'm' + curNumber;
                continue;
            }
            
            prevOper = true;
            if(curNumber.size() != 0)
            {
                result.append(curNumber);
                result+='|';//разделитель чисел
                curNumber.clear();
            }
            //операция или скобка
            int priority = opPriority[c];
            //если операция в стеке по приоритету ниже, то записываем текущую операцию
            if(s.size() == 0 || opPriority[s.top()] < priority )
                s.push(c);
            else
            {
                //пока приоритет операций в стеке Больше или равен текущей, забираем значения
                // и записываем их в строку
                while( s.size() >0 && opPriority[s.top()] >= priority )
                {
                    result+= s.top();
                    s.pop();
                }
                
                s.push(c);//не забудем саму текущую операцию занести
            }
            
        }
        else if(c == '(')
        {
            s.push(c);
            prevOper = true;
        }
        else if(c ==')')
        {
            if(curNumber.size() != 0)
            {
                result.append(curNumber);
                result+='|';//разделитель чисел
                curNumber.clear();
            }
            //извлекаем значения до тех пор, пока не встретим (
            while(s.top() != '(')
            {
                result+= s.top();
                s.pop();
            }
            
            s.pop();//и саму скобку уберем
            
            prevOper = false;
        }
                 
    }
    
    if(curNumber.size() !=0)
    {
        result += curNumber;
        result+='|';//разделитель чисел
    }
    
    while(s.size() !=0)
    {
        result+= s.top();
        s.pop();
    }
    
    //todo последним плюс записывать, если не знак операции
    
    return result;
}

float getNumber(const string numStr)
{
    regex r("([0-9]*\\.?[0-9]+)");
    std::smatch m1;
    std::regex_search(numStr, m1, r);
    if(m1.size() == 2)
    {
        bool isNegative = numStr[0] == 'm';
        return stof(m1[1])*(isNegative ? -1 :1);
    }
    else
        return FLT_MAX;
}

float calc(const float left, const float right, const char oper,string &err)
{
    if(oper == '+')
        return left +right;
    if(oper == '-')
        return left - right;
    if(oper == '*')
        return left*right;
    if(oper == '/')
    {
        if(static_cast<int>(right) ==0)
        {
            err = "divide by 0!";
            return FLT_MAX;
        }
        else
            return left/right;
    }
    
    err = "unknown opration!";
    return FLT_MAX;
}

float parseReversePolska(const string expression, string &errStr)
{
    stack<float> results;
    string curNumber;
    
    numSymbols.push_back('m');//это признак отрицательного числа
    
    for(int i = 0;i < expression.size();i++)
    {
        char c = expression[i];
        //считаем все символы числа
        if(find(numSymbols.begin(), numSymbols.end(), c) != numSymbols.end())
        {
            curNumber+=c;
            continue;
        }
        else if(c == '|')
        {
            //число закончилось, заносим значение в стек
            results.push(getNumber(curNumber));
            curNumber.clear();
        }
        else if(find(operationOnly.begin(), operationOnly.end(), c) != operationOnly.end())//если знак операции
        {
            float first = results.top();
            results.pop();
            float second = results.top();
            results.pop();
            
            float res = calc(second,first,c,errStr);
            results.push(res);
        }
    }
    
    return results.top();
}


#endif /* adStringParser_h */
