#ifndef _CONDEXP_H_
#define _CONDEXP_H_
#include "boolexp.h"
#include <string.h>

using namespace std;
class CondExp : public BoolExp
{
    shared_ptr<BoolExp> _operand1;
    shared_ptr<BoolExp> _operand2;
protected:
    //virtual shared_ptr<BoolExp> PCopy() const;
    virtual void SwapDependentVar(Subject,Subject);
    virtual shared_ptr<BoolExp> NoEvalCopy();
public:
    CondExp ( shared_ptr<BoolExp>, shared_ptr<BoolExp> );
    virtual ~CondExp ( );

    virtual string Name() const; 	//return a copy of the name
    virtual bool Evaluate(set<tuple<string,string,bool>>,set<string>,set<string>) const;
    virtual bool Evaluate(vector<tuple<string,string,bool>>,vector<string>,vector<string>) const;
    virtual bool Evaluate();
    virtual bool TestEvaluate(vector<tuple<string,string,bool>>&,vector<string>&,vector<string>&) const ;

    virtual void AssignChildren(bool);
    virtual shared_ptr<BoolExp> Copy();
    virtual BoolReturn Infer();
    virtual void SwapDependentVar(Subject,Subject,bool);
    virtual bool InUniverse();
    virtual string ReturnDependentVar() const;

};

#endif
