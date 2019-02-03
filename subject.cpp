#include "subject.h"
Subject::Subject(char var,unsigned num)
    :_variable(var),_number(num)
{
    CheckVariable(var);
}
Subject::Subject(string str,unsigned num)
    :_variable(str[0]),_number(num)
{
    CheckVariable(str[0]);
}
Subject::Subject(string str)
    : _variable(str[0])
{
    CheckVariable(str[0]);
    if (str.size() == 2)
    {
        cout << "RUNTIME-ERROR: str=" << str << endl;
        throw runtime_error("RUNTIME-ERROR:Subject::Subject(string str): variable string too short to be well formed");
    }
    else if (str.size() > 1&&str[1]!='_')
        throw runtime_error("RUNTIME-ERROR:Subject::Subject(string str): variable strings longer than one character must be a letter followed by an underscore followed by a number.");
    else if (str.size() > 1)
    {
        string tmp = str.substr(2,str.size()-2);
        unsigned num = atol(tmp.c_str());
        _number=num;
    }
    else
        _number=0;

}
Subject::Subject(char chr)
{
    CheckVariable(chr);
    _variable = chr;
    _number = 0;
}
Subject::~Subject ( )
{}
char Subject::Var() const
{
    return _variable;
}
unsigned Subject::Number() const
{
    return _number;
}
string Subject::Name() const
{
    string out = "";
    out += _variable;
    if(_number!=0)
    {
        out+="_";
        out += to_string(_number);
    }
    return out;
}
void Subject::CheckVariable(char chr) const
{
    if (isupper(chr)) throw runtime_error("RUNTIME-ERROR: bool Subject::CheckVariable(char chr) const: chr="+to_string(chr)+" is not lower case");
    if (!isalpha(chr)) throw runtime_error("RUNTIME-ERROR: bool Subject::CheckVariable(char chr) const: chr="+to_string(chr)+" is not alphanumeric");
}

