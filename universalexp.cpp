#include "universalexp.h"
UniversalExp::UniversalExp(shared_ptr<BoolExp> exp,Subject depvar)
    : BoolExp(UNIV_EXP),_child(exp),_var(depvar)
{}
UniversalExp::~UniversalExp( )
{}
string UniversalExp::Name() const
{
    string ret="(All ";
    ret+=_var.Name();
    ret += ")"+_child->Name();
    //if (_child->Type()==VAR_EXP) ret += ")"+_child->Name();
    //else ret+=+")("+_child->Name()+")";
    return ret;
}
bool UniversalExp::InUniverse()
{
    //TODO: Should this be checked?
    return true;
}

//TODO: what is the correct version of this?
bool UniversalExp::Evaluate()
{
    shared_ptr<BoolExp> testexp;
    for (auto it : _universesubjects)
    {
        //build expression from iterator without adding a new expression to the _universe
        //testexp=_child->PCopy();
        testexp=_child->NoEvalCopy();
        testexp->SwapDependentVar(_var,it);
        //cout << "NEW NAME: " << testexp->Name()<< endl;
        //if (!testexp->InUniverse()) continue; //skip formulas not in the universe TODO: this is probably something I don't want to avoid
        if (!testexp->Evaluate()) return false;
        /*else //this won't work for more complex formulas
        {
        	string usrchoice = false;
        	cout << "UNIVERSE-ERROR: Not enough information to evaluate: "
        	     << testexp->Name()
        		 << " please choose a true or false valuation (T/F):"
        		 << endl;
        	cin >> usrchoice;
        	if (usrchoice=="T"||usrchoice=="t") testexp->Assignment(true);
        	else testexp->Assignment(false);
        }*/
        //else cout << "UNKNOWN VALUE! " << testexp->Name()<< endl;
        //else throw runtime_error("RUNTIME-ERROR:bool UniversalExp::Evaluate() const: Reached illegal state in evaluation");
    }
    //if we make it here the formula was true
    return true;
}
bool UniversalExp::Evaluate(set<tuple<string,string,bool>> univ,set<string>subj,set<string>pred) const
{
    shared_ptr<BoolExp> testexp;
    for (auto it : subj)
    {
        testexp=_child->NoEvalCopy();
        testexp->SwapDependentVar(_var,it);
        if (!testexp->Evaluate(univ,subj,pred)) return false;
    }
    return true;
}
bool UniversalExp::Evaluate(vector<tuple<string,string,bool>>univ,vector<string> subj,vector<string> pred) const
{
    shared_ptr<BoolExp> testexp;
    for (auto it : subj)
    {
        testexp=_child->NoEvalCopy();
        testexp->SwapDependentVar(_var,it);
        if (!testexp->Evaluate(univ,subj,pred)) return false;
    }
    return true;
}
bool UniversalExp::TestEvaluate(vector<tuple<string,string,bool>>&univ,vector<string>&subj,vector<string>&pred) const
{
    shared_ptr<BoolExp> testexp;
    for (auto it : subj)
    {
        //cout << "DEBUG: test eval:" << testexp->Name() << endl;
        testexp=_child->NoEvalCopy();
        testexp->SwapDependentVar(_var,it);
        !testexp->TestEvaluate(univ,subj,pred);
        //if (!testexp->TestEvaluate(univ,subj,pred)) return false;
    }
    return true;
}
shared_ptr<BoolExp> UniversalExp::NoEvalCopy()
{
    return shared_ptr<BoolExp>(new UniversalExp(_child->NoEvalCopy(),_var));
}
shared_ptr<BoolExp> UniversalExp::Copy()
{
    return shared_ptr<BoolExp>(new UniversalExp(_child->Copy(),_var));
}
/*shared_ptr<BoolExp> UniversalExp::PCopy() const
{
    return shared_ptr<BoolExp>(new UniversalExp(_child->PCopy(),_var));
}*/
BoolReturn UniversalExp::Infer()
{
    //throw runtime_error("RUNTIME-ERROR:BoolReturn UniversalExp::Infer(): Not implemented");
    return BoolReturn(_child->NoEvalCopy(),_child->NoEvalCopy());
}
void UniversalExp::SwapDependentVar(Subject src,Subject dst)
{
    if (_var.Name()==src.Name())_var=dst;
    _child->SwapDependentVar(src,dst);
}
void UniversalExp::SwapDependentVar(Subject src,Subject dst,bool val)
{
    if (_var.Name()==src.Name())_var=dst;
    _child->SwapDependentVar(src,dst,val);
}
string UniversalExp::ReturnDependentVar() const
{
    return _var.Name();
}
//TODO: is this safe?
void UniversalExp::AssignChildren(bool val)
{
    //_child->AssignChildren(val);
    return;
}

