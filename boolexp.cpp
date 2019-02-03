#include "boolexp.h"
set<string> BoolExp::_universepredicates;
//set<tuple<string,string,bool>> BoolExp::_universe;
set<tuple<string,string,bool>> BoolExp::_universe;
set<string> BoolExp::_universesubjects;

BoolReturn::BoolReturn(shared_ptr<BoolExp> o1, shared_ptr<BoolExp> o2)
    :op1(o1), op2(o2)
{}

BoolExp::BoolExp (PropType p)
    : _type(p)
{}
BoolExp::~BoolExp ( )
{}
PropType BoolExp::Type() const
{
    return _type;
}
string BoolExp::SubjectUniverse() const
{
    if (_universesubjects.empty()) return "{}";
    string out = "{";
    for (auto it : _universesubjects)
    {
        out += it;
        out += ", ";
    }
    out[out.size()-2]='}';
    return out;
}
string BoolExp::PredicateUniverse() const
{
    if (_universepredicates.empty()) return "{}";
    string out = "{";
    for (auto it : _universepredicates)
    {
        out += it;
        out += ", ";
    }
    out[out.size()-2]='}';
    //out[out.size()-1]='}';
    return out;
}
string BoolExp::Universe() const
{
    if (_universe.empty()) return "{}";
    string out = "{";
    for (auto it : _universe)
    {
        out += "(";
        out += get<0>(it);
        out += ",";
        out += get<1>(it);
        out += ",";
        //out += (get<2>(it))?("T"):("F");
        //out += (get<2>(it))?("1"):("0");
        //out += (get<2>(it))?("True"):("False");
        if (get<2>(it)==true) out+="TRUE";
        else out+="FALSE";
        //else if (get<2>(it)==FALSE) out+="FALSE";
        //else out+="UNKNOWN";
        out += "), ";
    }
    out[out.size()-2]='}';
    //out[out.size()-1]='}';
    return out;
}
void BoolExp::ClearUniverse()
{
    _universe.clear();
    _universesubjects.clear();
    _universepredicates.clear();
}
//TODO: this can get called at unexpected times is the error checking in this appropriate?
void BoolExp::AddTupleToUniverse(string pred,vector<Subject> subj,bool val)
{
    //error checking
    //if (!isupper(pred.c_str()[0])||pred.size()>1)
    if (!isupper(pred.c_str()[0]))
        throw runtime_error("RUNTIME-ERROR:void BoolExp::AddTupleToUniverse(string pred,vector<Subject> subj,bool val):Predicates must begin with an upper case letter.");
    for (auto it : subj)
        if (!(islower(it.Var())))
            throw runtime_error("RUNTIME-ERROR:void BoolExp::AddTupleToUniverse(string pred,vector<Subject> subj,bool val):Subjects must be a single lower case letter optionally followed by an underscore and a number.");

    //insert each subject
    for (auto it : subj) _universesubjects.insert(it.Name());
    //insert the predicate
    _universepredicates.insert(pred);

    //collect all the subjects into a string representing the collection
    string collectedsubj = StringifySubjects(subj);

    auto true_tuple = _universe.find(make_tuple(pred,collectedsubj,true));
    auto false_tuple = _universe.find(make_tuple(pred,collectedsubj,false));
    //auto unk_tuple = _universe.find(make_tuple(pred,subj,UNKNOWN));
    //if (true_tuple!=_universe.end()||false_tuple!=_universe.end()||unk_tuple!=_universe.end())
    //if (true_tuple!=_universe.end()||false_tuple!=_universe.end())
    if ( (!val&&true_tuple!=_universe.end()) || (val&&false_tuple!=_universe.end()))
    {
        cout << endl << endl;
        cout << pred << collectedsubj << " is " << val << endl;
        cout << Universe() << endl;
        if (true_tuple!=_universe.end()) cout << "if (true_tuple!=_universe.end()) is true " << endl;
        if (false_tuple!=_universe.end()) cout << "if (false_tuple!=_universe.end()) is true " << endl;
        throw runtime_error("RUNTIME-ERROR:void BoolExp::AddTupleToUniverse(string pred,string subj,bool val): Inserting contradiction into universe.");
    }
    else
        _universe.insert(make_tuple(pred,collectedsubj,val));

    //if necessary insert
    //if(_universe.find(make_tuple(pred,subj,BOOL(val)))==_universe.end())
    //    _universe.insert(make_tuple(pred,subj,BOOL(val)));
    //else
    //    throw runtime_error("RUNTIME-ERROR:void BoolExp::AddTupleToUniverse(string pred,string subj,bool val): Inserting contradiction into universe.");

}
void BoolExp::AssignTupleInUniverse(string pred,vector<Subject> subj,bool val)
{
    //error checking
    if (!isupper(pred.c_str()[0])||pred.size()>1)
        throw runtime_error("RUNTIME-ERROR:VarExp::VarExp(string pred,string subj,bool val):Predicates are restricted to upper case single letters.");
    for (auto it : subj)
        if (!islower(it.Var()))
            throw runtime_error("RUNTIME-ERROR:VarExp::VarExp(string pred,string subj,bool val):Subjects are restricted to one or more lower case letters.");

    //insert each subject
    for (auto it : subj) _universesubjects.insert(it.Name());
    _universepredicates.insert(pred);

    //collect all the subjects into a string representing the collection
    string collectedsubj = StringifySubjects(subj);

    auto true_tuple = _universe.find(make_tuple(pred,collectedsubj,true));
    auto false_tuple = _universe.find(make_tuple(pred,collectedsubj,false));
    //auto unk_tuple = _universe.find(make_tuple(pred,subj,UNKNOWN));
    if (true_tuple!=_universe.end())
    {
        _universe.erase(true_tuple);
    }
    if (false_tuple!=_universe.end())
    {
        _universe.erase(false_tuple);
    }
    //if (unk_tuple!=_universe.end())
    //{
    //    _universe.erase(unk_tuple);
    //}
    _universe.insert(make_tuple(pred,collectedsubj,val));
}
string BoolExp::StringifySubjects(vector<Subject> vec) const
{
    string retstr = "(";
    for (auto it : vec) retstr += (it.Name()+",");
    retstr[retstr.size()-1]=')';
    return retstr;
}
//TODO: these are set to throw for debugging purposes
set<string> BoolExp::UniverseSubjectsCopy() const
{
    return set<string>(_universesubjects);
    throw runtime_error("RUNTIME-ERROR:set<string> BoolExp::UniverseSubjectsCopy() const: Not yet implemented");
}
void BoolExp::UniverseSubjectsAssign(set<string> inset)
{
    _universesubjects=inset;
    throw runtime_error("RUNTIME-ERROR:void BoolExp::UniverseSubjectsAssign(set<string> inset): Not yet implemented");
}
void BoolExp::UniverseSubjectsAssign(vector<string> invec)
{
    UniverseSubjectsAssign(set<string>(invec.begin(),invec.end()));
    throw runtime_error("RUNTIME-ERROR:void BoolExp::UniverseSubjectsAssign(vector<string> invec): Not yet implemented");
}

set<string> BoolExp::UniversePredicatesCopy() const
{
    return set<string>(_universepredicates);
    throw runtime_error("RUNTIME-ERROR:set<string> BoolExp::UniversePredicatesCopy() const: Not yet implemented");
}
void BoolExp::UniversePredicatesAssign(set<string> inset)
{
    _universepredicates=inset;
    throw runtime_error("RUNTIME-ERROR:void BoolExp::UniversePredicatesAssign(set<string> inset): Not yet implemented");
}
void BoolExp::UniversePredicatesAssign(vector<string> invec)
{
    UniversePredicatesAssign(set<string>(invec.begin(),invec.end()));
    throw runtime_error("RUNTIME-ERROR:void BoolExp::UniversePredicatesAssign(vector<string> invec): Not yet implemented");
}

set<tuple<string,string,bool>> BoolExp::UniverseCopy() const
{
    return set<tuple<string,string,bool>>(_universe);
    throw runtime_error("RUNTIME-ERROR:set<tuple<string,string,bool>> BoolExp::UniverseCopy() const: Not yet implemented");
}
void BoolExp::UniverseAssign(set<tuple<string,string,bool>> inset)
{
    _universe=inset;
    throw runtime_error("RUNTIME-ERROR:void BoolExp::UniverseAssign(set<tuple<string,string,bool>> inset): Not yet implemented");
}
void BoolExp::UniverseAssign(vector<tuple<string,string,bool>> invec)
{
    UniverseAssign(set<tuple<string,string,bool>>(invec.begin(),invec.end()));
    throw runtime_error("RUNTIME-ERROR:void BoolExp::UniverseAssign(vector<tuple<string,string,bool>> invec): Not yet implemented");
}

string BoolExp::Predicate() const
{
    throw runtime_error("RUNTIME-ERROR:string BoolExp::Predicate() const: "+Name()+" is not a VarExp");
}
string BoolExp::Subjects() const
{
    throw runtime_error("RUNTIME-ERROR:string BoolExp::Subjects() const: "+Name()+" is not a VarExp");
}
vector<Subject> BoolExp::SubjectVector() const
{
    throw runtime_error("RUNTIME-ERROR:vector<Subject> BoolExp::SubjectVector() const: "+Name()+" is not a VarExp");
}

