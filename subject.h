#ifndef _SUBJECT_WPV
#define _SUBJECT_WPV
#include <string>
#include <iostream>
//#include <memory>
//#include <vector>
//#include <tuple>
//#include <set>
using namespace std;
//subjects are variables
//they are named using the lowercase letters and a number x_0,x_1,x_2,...
class Subject
{
    //string _variable;
    char _variable;
    unsigned _number;
public:
    Subject(char,unsigned);
    Subject(string,unsigned);
    Subject(string);
    Subject(char);
    ~Subject ( );
    string Name() const;
    char Var() const;
    unsigned Number() const;
    void CheckVariable(char chr) const;
};
#endif
