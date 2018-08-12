//
//  main.cpp
//  calculator
//
//  Created by Аня Дубровина on 11.08.2018.
//  Copyright © 2018 Анна Дубровина. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <regex>
#include "arStringParser.h"

using namespace std;


int main(int argc, const char * argv[]) {
    // insert code here...
    
    char expStr[1024];
    cout<<"put expression:";
    cin.getline(expStr,sizeof(expStr));
    
    string expression = expStr;
    if(!checkString(expression))
        return 0;
    
    //cout<<"given str:"<<expression<<endl;
    
    //calculateResult(expression);
    string rev =toReversePolska(expression);
    //cout<<"reverse polska="<<rev;
    string errStr;
    float res = parseReversePolska(rev, errStr);
    
    if(errStr.size() != 0)
        cout<<"err:"<<errStr<<endl;
    else
    {
        cout.precision(2);
        cout<<"result: "<<res<<endl;
    }
    
    return 0;
}
