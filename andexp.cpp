#include "andexp.h"
AndExp::AndExp ( shared_ptr<BoolExp> op1, shared_ptr<BoolExp> op2)
    : BoolExp(AND_EXP)
{
    _operand1 = op1;
    _operand2 = op2;
}

AndExp::~AndExp ( )
{}

bool AndExp::Evaluate()
{
    bool val1 = _operand1->Evaluate();
    bool val2 = _operand2->Evaluate();

    return val1 && val2;
}
bool AndExp::Evaluate(set<tuple<string,string,bool>> univ,set<string>subj,set<string>pred) const
{
    bool val1 = _operand1->Evaluate(univ,subj,pred);
    bool val2 = _operand2->Evaluate(univ,subj,pred);

    return val1 && val2;
}
bool AndExp::Evaluate(vector<tuple<string,string,bool>> univ,vector<string>subj,vector<string>pred) const
{
    bool val1 = _operand1->Evaluate(univ,subj,pred);
    bool val2 = _operand2->Evaluate(univ,subj,pred);

    return val1 && val2;
}
bool AndExp::TestEvaluate(vector<tuple<string,string,bool>>& univ,vector<string>&subj,vector<string>&pred) const
{
    bool val1 = _operand1->TestEvaluate(univ,subj,pred);
    bool val2 = _operand2->TestEvaluate(univ,subj,pred);

    return val1 && val2;
}
bool AndExp::InUniverse()
{
    return _operand1->InUniverse()&&_operand2->InUniverse();
}

shared_ptr<BoolExp> AndExp::NoEvalCopy()
{
    return shared_ptr<BoolExp>(new AndExp(_operand1->NoEvalCopy(),
                                          _operand2->NoEvalCopy()));
}
shared_ptr<BoolExp> AndExp::Copy()
{
    return shared_ptr<BoolExp>(new AndExp(_operand1->Copy(),
                                          _operand2->Copy()));
}
/*shared_ptr<BoolExp> AndExp::PCopy() const
{
    return shared_ptr<BoolExp>(new AndExp(_operand1->PCopy(),
                                          _operand2->PCopy()));
}*/

string AndExp::Name() const
{
    string ret = "(" + _operand1->Name() + " & " + _operand2->Name() + ")";
    return ret;
}
BoolReturn AndExp::Infer()
{
    return BoolReturn(_operand1->NoEvalCopy(),_operand2->NoEvalCopy());
}
void AndExp::SwapDependentVar(Subject src,Subject dst)
{
    _operand1->SwapDependentVar(src,dst);
    _operand2->SwapDependentVar(src,dst);
}
void AndExp::SwapDependentVar(Subject src,Subject dst,bool val)
{
    _operand1->SwapDependentVar(src,dst,val);
    _operand2->SwapDependentVar(src,dst,val);
}
string AndExp::ReturnDependentVar() const
{
    throw runtime_error("RUNTIME-ERROR:string AndExp::ReturnDependentVar() const: Unimplemented call");
}
void AndExp::AssignChildren(bool val)
{
    _operand1->AssignChildren(val);
    _operand2->AssignChildren(val);
}

