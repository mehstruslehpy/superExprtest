#ifndef _NOTEXP_H_
#define _NOTEXP_H_
#include "boolexp.h"
#include <string.h>

using namespace std;

class NotExp : public BoolExp
{
    shared_ptr<BoolExp> _operand;
protected:
    //virtual shared_ptr<BoolExp> PCopy() const;
    virtual void SwapDependentVar(Subject,Subject);
    virtual shared_ptr<BoolExp> NoEvalCopy();
public:
    NotExp ( shared_ptr<BoolExp> op);
    virtual ~NotExp ( );
    virtual string Name() const;
    virtual bool Evaluate();
    virtual bool Evaluate(set<tuple<string,string,bool>>,set<string>,set<string>) const;
    virtual bool Evaluate(vector<tuple<string,string,bool>>,vector<string>,vector<string>) const;
    virtual bool TestEvaluate(vector<tuple<string,string,bool>>&,vector<string>&,vector<string>&) const ;

    virtual void AssignChildren(bool);
    virtual shared_ptr<BoolExp> Copy();
    virtual BoolReturn Infer();
    virtual void SwapDependentVar(Subject,Subject,bool);
    virtual bool InUniverse();
    virtual string ReturnDependentVar() const;


};

#endif
