#ifndef _UNIVEXP_H_
#define _UNIVEXP_H_
#include "boolexp.h"
#include <string.h>
#include <memory>
using namespace std;

class UniversalExp : public BoolExp
{
    shared_ptr<BoolExp> _child;
    Subject _var;
protected:
    //virtual shared_ptr<BoolExp> PCopy() const;
    virtual void SwapDependentVar(Subject,Subject);
    virtual shared_ptr<BoolExp> NoEvalCopy();
public:
    UniversalExp (shared_ptr<BoolExp>,Subject);
    virtual ~UniversalExp ( );
    virtual string Name() const;
    virtual bool Evaluate();
    virtual bool Evaluate(set<tuple<string,string,bool>>,set<string>,set<string>) const;
    virtual bool Evaluate(vector<tuple<string,string,bool>>,vector<string>,vector<string>) const;
    virtual bool TestEvaluate(vector<tuple<string,string,bool>>&,vector<string>&,vector<string>&) const ;

    virtual void AssignChildren(bool);
    virtual shared_ptr<BoolExp> Copy();
    virtual BoolReturn Infer();
    virtual void SwapDependentVar(Subject,Subject,bool);
    virtual string ReturnDependentVar() const;

    virtual bool InUniverse();

};
#endif
