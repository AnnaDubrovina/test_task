//
//  main.cpp
//  CalculatorTest
//
//  Created by Аня Дубровина on 12.08.2018.
//  Copyright © 2018 Анна Дубровина. All rights reserved.
//

#include <iostream>
#include <cassert>
#include <cmath>
#include "../arStringParser.h"

float calculateExpression(string &str)
{
    if(checkString(str))
    {
        auto res = toReversePolska(str);
        string errStr;
        auto r =  parseReversePolska(res, errStr);
        if(errStr.size() !=0)
            cout<<errStr;
        
        return r;
    }
    return FLT_MAX;
}


int main(int argc, const char * argv[]) {
    // insert code here...
    
    //сначала проверка строкового выражения
    string s1 = "1+4*(-4.7+5.4)";
    assert(checkString(s1));
    
    s1 = "1,3 +    1.6";
    assert(checkString(s1));
    
    s1 = "1.r +1";
    assert(checkString(s1) == false);
    
    s1 = "-1.5 +1";
    assert(checkString(s1) == true);
    
    s1 = "(-1.5 +1";
    assert(checkString(s1) == false);
    
    //теперь проверка преобразования к обратной польской записи
    s1="-1*(4-6)+7";
    string res = toReversePolska(s1);
    assert(strcmp(res.c_str(),"m1|4|6|-*7|+") ==0);
    
    s1="1.3+1.6";
    res = toReversePolska(s1);
    assert(strcmp(res.c_str(),"1.3|1.6|+") ==0);
    
    s1="-1-1-1-1";
    float a= -4;
    assert(calculateExpression(s1) == a );
    
    s1="-1*(4-6)+7";
    a= 9;
    assert(calculateExpression(s1) == a );
    
    s1="-1+5-3";
    a= 1;
    assert(calculateExpression(s1) == a );
    
    s1="-10+(8*2.5)-(3/1,5)";
    a= 8;
    assert(calculateExpression(s1) == a );
    
    s1="1+(2*(2.5+2.5+(3-2)))-(3/1.5)";
    a= 11;
    assert(calculateExpression(s1) == a );
    
    s1="2/(3-3)";
    a= FLT_MAX;
    assert(calculateExpression(s1) == a );
    
    s1="1/3";
    a= 0.33;
    assert(fabs(calculateExpression(s1) - a)<0.01  );
    
    //работа с отрицательными числами
    s1="1+-8";
    a= -7;
    assert(calculateExpression(s1) == a );
    
    cout<<"tests OK";
    
    return 0;
}
