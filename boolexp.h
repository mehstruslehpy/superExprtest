#ifndef _BOOLEXP_H_
#define _BOOLEXP_H_
#include "subject.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <set>
#include <vector>
using namespace std;

//fwd declaration for boolreturn
class BoolExp;

enum PropType
{
    /**The type for an OR expression*/
    OR_EXP,
    /**The type for a Variable expression*/
    VAR_EXP,
    /**The type for a NOT expression*/
    NOT_EXP,
    /**The type for an AND expression*/
    AND_EXP,
    /**The type for an Conditional expression*/
    COND_EXP,
    /**The type for a universally quantified expression*/
    UNIV_EXP,
    /**The type for an existentially quantified expression*/
    EXIST_EXP
};

struct BoolReturn
{
    BoolReturn(shared_ptr<BoolExp>,shared_ptr<BoolExp>);
    ~BoolReturn() {}
    shared_ptr<BoolExp> op1;
    shared_ptr<BoolExp> op2;
};

using namespace std;
//TODO: pick what stuff to make protected in derived
class BoolExp
{
    PropType _type;
protected:
    static set<string> _universepredicates; //the universe of predicates
    static set<string> _universesubjects; //the universe of subjects
    //static set<Subject> _universesubjects; //the universe of subjects
    static set<tuple<string,string,bool>> _universe; //(predicate,subject,value)
    //static set<tuple<string,subject,bool>> _universe; //(predicate,subject,value)
public:
    BoolExp (PropType);
    virtual ~BoolExp ( );
    virtual string Name() const = 0;
    virtual bool Evaluate() = 0; //evaluate with only true or false values being returned
    virtual void AssignChildren(bool) = 0; //assign all children varexp's to the argument
    virtual bool Evaluate(set<tuple<string,string,bool>>,set<string>,set<string>) const = 0; //evaluate with the specified universe (univ,subj,pred)
    virtual bool Evaluate(vector<tuple<string,string,bool>>,vector<string>,vector<string>) const = 0; //evaluate with the specified universe (univ,subj,pred)
    virtual bool TestEvaluate(vector<tuple<string,string,bool>>&,vector<string>&,vector<string>&) const = 0;

    //virtual shared_ptr<BoolExp> PCopy() const = 0; //create a copy of the formula without altering the _universe will be protected in derived classes
    virtual shared_ptr<BoolExp> Copy() = 0;
    virtual shared_ptr<BoolExp> NoEvalCopy() = 0;
    virtual PropType Type() const;
    virtual BoolReturn Infer() = 0;

    virtual void SwapDependentVar(Subject,Subject) = 0; //all instances of arg1 become arg2 in all subformulas does nothing to _universe
    virtual bool InUniverse() = 0; //check if all relations in all subformulas are in the _universe

    //all instances of arg1 become arg2 in all subformulas assigns a value if the resulting formula is not already assigned
    virtual void SwapDependentVar(Subject,Subject,bool) = 0;
    virtual string ReturnDependentVar() const = 0;

    //TODO:
    //virtual void SwapDependentVar(char,char,bool) = 0;
    string SubjectUniverse() const; //print the universe of subjects
    //set<string> UniverseSubjectsCopy() const;
    set<string> UniverseSubjectsCopy() const;
    void UniverseSubjectsAssign(set<string>);
    void UniverseSubjectsAssign(vector<string>);

    string PredicateUniverse() const; //print the universe of predicates
    //set<string> UniversePredicatesCopy() const;
    set<string> UniversePredicatesCopy() const;
    void UniversePredicatesAssign(set<string>);
    void UniversePredicatesAssign(vector<string>);

    string Universe() const; //print the universe of tuples
    //set<tuple<string,string,bool>> UniverseCopy() const;
    set<tuple<string,string,bool>> UniverseCopy() const;
    void UniverseAssign(set<tuple<string,string,bool>>);
    void UniverseAssign(vector<tuple<string,string,bool>>);

    string StringifySubjects(vector<Subject>) const; //turns a vector of subjects into a string
    //string KnownUniverse() const; //print only the known parts of the universe
    void ClearUniverse();
    void AddTupleToUniverse(string,vector<Subject>,bool); //add to the universe without actually making an expression throw on inserting contradictions
    //TODO: need remove tuple from universe function
    void AssignTupleInUniverse(string,vector<Subject>,bool); //add to the universe without actually making an expression reassign on inserting contradictions
    //TODO: should I deal with unknowns or not?
    //void AddToUniverse(string,string,BOOL); //add to the universe without actually making an expression

    //if VarExp return the requested info else throw
    virtual string Predicate() const;
    virtual string Subjects() const;
    virtual vector<Subject> SubjectVector() const;

};
#endif
