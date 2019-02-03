#include "orexp.h"
OrExp::OrExp ( shared_ptr<BoolExp> op1, shared_ptr<BoolExp> op2)
    : BoolExp(OR_EXP)
{
    _operand1 = op1;
    _operand2 = op2;
}

OrExp::~OrExp ( )
{}

bool OrExp::Evaluate()
{
    bool val1 = _operand1->Evaluate();
    bool val2 = _operand2->Evaluate();

    return val1 || val2;
}
bool OrExp::Evaluate(set<tuple<string,string,bool>> univ,set<string>subj,set<string>pred) const
{
    bool val1 = _operand1->Evaluate(univ,subj,pred);
    bool val2 = _operand2->Evaluate(univ,subj,pred);

    return val1 || val2;
}
bool OrExp::Evaluate(vector<tuple<string,string,bool>> univ,vector<string>subj,vector<string>pred) const
{
    bool val1 = _operand1->Evaluate(univ,subj,pred);
    bool val2 = _operand2->Evaluate(univ,subj,pred);

    return val1 || val2;
}
bool OrExp::TestEvaluate(vector<tuple<string,string,bool>>&univ,vector<string>&subj,vector<string>&pred) const
{
    bool val1 = _operand1->TestEvaluate(univ,subj,pred);
    bool val2 = _operand2->TestEvaluate(univ,subj,pred);

    return val1 || val2;
}

bool OrExp::InUniverse()
{
    return _operand1->InUniverse()&&_operand2->InUniverse();
}

shared_ptr<BoolExp> OrExp::NoEvalCopy()
{
    return shared_ptr<BoolExp>(new OrExp(_operand1->NoEvalCopy(),
                                         _operand2->NoEvalCopy()));
}

shared_ptr<BoolExp> OrExp::Copy()
{
    return shared_ptr<BoolExp>(new OrExp(_operand1->Copy(),
                                         _operand2->Copy()));
}
/*shared_ptr<BoolExp> OrExp::PCopy() const
{
    return shared_ptr<BoolExp>(new OrExp(_operand1->PCopy(),
                                         _operand2->PCopy()));
}*/
string OrExp::Name() const
{
    string ret = "(" + _operand1->Name() + " v " + _operand2->Name() + ")";
    return ret;
}
BoolReturn OrExp::Infer()
{
    return BoolReturn(_operand1->NoEvalCopy(),_operand2->NoEvalCopy());
}
void OrExp::SwapDependentVar(Subject src,Subject dst)
{
    _operand1->SwapDependentVar(src,dst);
    _operand2->SwapDependentVar(src,dst);
}
void OrExp::SwapDependentVar(Subject src,Subject dst,bool val)
{
    _operand1->SwapDependentVar(src,dst,val);
    _operand2->SwapDependentVar(src,dst,val);
}
string OrExp::ReturnDependentVar() const
{
    throw runtime_error("RUNTIME-ERROR:string OrExp::ReturnDependentVar() const: Unimplemented call");
}
void OrExp::AssignChildren(bool val)
{
    _operand1->AssignChildren(val);
    _operand2->AssignChildren(val);
}
