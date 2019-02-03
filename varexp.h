#ifndef _VAREXP_H_
#define _VAREXP_H_
#include "boolexp.h"
#include <string.h>
#include <memory>
#include <vector>
#include <tuple>
#include <set>
#include <sstream>
#include <algorithm>
//TODO: do I want to add metavarexp that don't affect the _universe?
using namespace std;

class VarExp : public BoolExp
{
    unsigned _instance; //might not need this
    string _predicate; //the current predicate
    vector<Subject> _subject; //the current subject/subjects
    unsigned _arity;
protected:
    //virtual shared_ptr<BoolExp> PCopy() const;
    virtual void SwapDependentVar(Subject,Subject);
    virtual shared_ptr<BoolExp> NoEvalCopy();
public:
    VarExp (string,vector<Subject>,bool);
    VarExp (string,vector<Subject>); //TODO: is this safe?
    virtual ~VarExp ( );
    virtual string Name() const;
    virtual bool Evaluate();
    virtual bool Evaluate(set<tuple<string,string,bool>>,set<string>,set<string>) const;
    virtual bool Evaluate(vector<tuple<string,string,bool>>,vector<string>,vector<string>) const;
    virtual bool TestEvaluate(vector<tuple<string,string,bool>>&,vector<string>&,vector<string>&) const ;

    virtual shared_ptr<BoolExp> Copy();
    virtual BoolReturn Infer();
    void Assignment(bool val);
    virtual void AssignChildren(bool);
    unsigned Arity() const;
    unsigned Instance() const;
    virtual string Predicate() const;
    virtual string Subjects() const;
    virtual vector<Subject> SubjectVector() const;
    virtual void SwapDependentVar(Subject,Subject,bool);
    virtual string ReturnDependentVar() const;
    virtual bool InUniverse();

};
#endif
