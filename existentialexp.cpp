#include "existentialexp.h"
ExistentialExp::ExistentialExp(shared_ptr<BoolExp> exp,Subject depvar)
    : BoolExp(EXIST_EXP),_child(exp),_var(depvar)
{}
ExistentialExp::~ExistentialExp( )
{}
string ExistentialExp::Name() const
{
    string ret="(Some ";
    ret+=_var.Name();
    ret += ")"+_child->Name();
    //if (_child->Type()==VAR_EXP) ret += ")"+_child->Name();
    //else ret+=+")("+_child->Name()+")";
    return ret;
}
bool ExistentialExp::InUniverse()
{
    //TODO: Should this be checked?
    return true;
}

bool ExistentialExp::Evaluate()
{
    shared_ptr<BoolExp> testexp;
    //there might be a useful optimization here that doesn't involve checking the whole universe
    //since I really only need at least one truth
    for (auto it : _universesubjects)
    {
        testexp=_child->NoEvalCopy();
        testexp->SwapDependentVar(_var,it);
        if (testexp->Evaluate()) return true;
    }
    //if we make it here the formula was false
    return false;
}
bool ExistentialExp::Evaluate(set<tuple<string,string,bool>>univ,set<string>subj,set<string>pred) const
{
    shared_ptr<BoolExp> testexp;
    for (auto it : subj)
    {
        testexp=_child->NoEvalCopy();
        testexp->SwapDependentVar(_var,it);
        if (testexp->Evaluate(univ,subj,pred)) return true;
    }
    return false;
}
bool ExistentialExp::Evaluate(vector<tuple<string,string,bool>>univ,vector<string>subj,vector<string>pred) const
{
    shared_ptr<BoolExp> testexp;
    for (auto it : subj)
    {
        testexp=_child->NoEvalCopy();
        testexp->SwapDependentVar(_var,it);
        if (testexp->Evaluate(univ,subj,pred)) return true;
    }
    return false;
}
bool ExistentialExp::TestEvaluate(vector<tuple<string,string,bool>>&univ,vector<string>&subj,vector<string>&pred) const
{
    shared_ptr<BoolExp> testexp;
    for (auto it : subj)
    {
        testexp=_child->NoEvalCopy();
        testexp->SwapDependentVar(_var,it);
        //if (testexp->TestEvaluate(univ,subj,pred)) return true;
        testexp->TestEvaluate(univ,subj,pred);
    }
    return false;
}
shared_ptr<BoolExp> ExistentialExp::NoEvalCopy()
{
    return shared_ptr<BoolExp>(new ExistentialExp(_child->NoEvalCopy(),_var));
}
shared_ptr<BoolExp> ExistentialExp::Copy()
{
    return shared_ptr<BoolExp>(new ExistentialExp(_child->Copy(),_var));
}
BoolReturn ExistentialExp::Infer()
{
    return BoolReturn(_child->NoEvalCopy(),_child->NoEvalCopy());
}
void ExistentialExp::SwapDependentVar(Subject src,Subject dst)
{
    if (_var.Name()==src.Name())_var=dst;
    _child->SwapDependentVar(src,dst);
}
void ExistentialExp::SwapDependentVar(Subject src,Subject dst,bool val)
{
    if (_var.Name()==src.Name())_var=dst;
    _child->SwapDependentVar(src,dst,val);
}
string ExistentialExp::ReturnDependentVar() const
{
    return _var.Name();
}
void ExistentialExp::AssignChildren(bool val)
{
    //_child->AssignChildren(val);
    return;
}
