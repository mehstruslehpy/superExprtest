#ifndef _ANDEXP_H_
#define _ANDEXP_H_
#include "boolexp.h"
#include <string.h>	//cstring functions

using namespace std;

class AndExp : public BoolExp
{
    shared_ptr<BoolExp> _operand1;
    shared_ptr<BoolExp> _operand2;
protected:
    //virtual shared_ptr<BoolExp> PCopy() const;
    virtual void SwapDependentVar(Subject,Subject);
    virtual shared_ptr<BoolExp> NoEvalCopy();
public:
    AndExp ( shared_ptr<BoolExp> op1, shared_ptr<BoolExp> op2);
    virtual ~AndExp ( );
    virtual string Name() const;
    virtual bool Evaluate(set<tuple<string,string,bool>>,set<string>,set<string>) const;
    virtual bool Evaluate(vector<tuple<string,string,bool>>,vector<string>,vector<string>) const;
    virtual bool TestEvaluate(vector<tuple<string,string,bool>>&,vector<string>&,vector<string>&) const ;
    virtual bool Evaluate();
    virtual void AssignChildren(bool);
    virtual shared_ptr<BoolExp> Copy();
    virtual BoolReturn Infer();
    virtual bool InUniverse();
    virtual void SwapDependentVar(Subject,Subject,bool);
    virtual string ReturnDependentVar() const;
};

#endif
