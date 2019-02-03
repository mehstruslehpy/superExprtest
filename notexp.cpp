#include "notexp.h"
NotExp::NotExp ( shared_ptr<BoolExp> op)
    : BoolExp(NOT_EXP)
{
    _operand = op;
}

NotExp::~NotExp ( )
{}

bool NotExp::Evaluate()
{
    return !_operand->Evaluate();
}
bool NotExp::Evaluate(set<tuple<string,string,bool>> univ,set<string> subj,set<string> pred) const
{
    return !_operand->Evaluate(univ,subj,pred);
}
bool NotExp::Evaluate(vector<tuple<string,string,bool>> univ,vector<string> subj,vector<string> pred) const
{
    return !_operand->Evaluate(univ,subj,pred);
}
bool NotExp::TestEvaluate(vector<tuple<string,string,bool>>&univ,vector<string>&subj,vector<string>&pred) const
{
    return !_operand->TestEvaluate(univ,subj,pred);
}
shared_ptr<BoolExp> NotExp::NoEvalCopy()
{
    return shared_ptr<BoolExp>(new NotExp(_operand->NoEvalCopy()));
}
shared_ptr<BoolExp> NotExp::Copy()
{
    return shared_ptr<BoolExp>(new NotExp(_operand->Copy()));
}
/*shared_ptr<BoolExp> NotExp::PCopy() const
{
    return shared_ptr<BoolExp>(new NotExp(_operand->PCopy()));
}*/
bool NotExp::InUniverse()
{
    return _operand->InUniverse();
}

string NotExp::Name() const
{
    return "~" + _operand->Name();
}
BoolReturn NotExp::Infer()
{
    //return BoolReturn(_operand, nullptr);
    return BoolReturn(_operand->NoEvalCopy(), _operand->NoEvalCopy());
}
void NotExp::SwapDependentVar(Subject src,Subject dst)
{
    _operand->SwapDependentVar(src,dst);
}
void NotExp::SwapDependentVar(Subject src,Subject dst,bool val)
{
    _operand->SwapDependentVar(src,dst,val);
}
string NotExp::ReturnDependentVar() const
{
    if (_operand->Type()==UNIV_EXP||_operand->Type()==EXIST_EXP) return _operand->ReturnDependentVar();
    //else return "";
    else throw runtime_error("RUNTIME-ERROR:string NotExp::ReturnDependentVar() const: Cannot query this expression for a dependent variable:"+Name());
}
void NotExp::AssignChildren(bool val)
{
    _operand->AssignChildren(val);
}
