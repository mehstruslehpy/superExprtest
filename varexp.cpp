#include "varexp.h"
//TODO: Keep this?
/*VarExp::VarExp (string pred,string subj)
    : BoolExp(VAR_EXP),_predicate(pred),_subject(subj),_arity(_subject.size())
{
    if (!isupper(pred.c_str()[0])||pred.size()>1)
        throw runtime_error("RUNTIME-ERROR:VarExp::VarExp(string pred,string subj,bool val):Predicates are restricted to upper case single letters.");
    for (auto it : subj)
        if (!islower(it))
            throw runtime_error("RUNTIME-ERROR:VarExp::VarExp(string pred,string subj,bool val):Subjects are restricted to one or more lower case letters.");
    _instance=_universe.size();
}*/
VarExp::VarExp (string pred,vector<Subject> subj,bool val)
    : BoolExp(VAR_EXP),_predicate(pred),_subject(subj),_arity(_subject.size())
{
    //if (!isupper(pred.c_str()[0])||pred.size()>1)
    if (!isupper(pred.c_str()[0]))
        throw runtime_error("RUNTIME-ERROR:VarExp::VarExp (string pred,vector<Subject> subj,bool val):Predicates must begin with an upper case letter.");
    //throw runtime_error("RUNTIME-ERROR:VarExp::VarExp(string pred,string subj,bool val):Predicates are restricted to upper case single letters.");
    /*for (auto it : subj)
        if (!islower(it))
            throw runtime_error("RUNTIME-ERROR:VarExp::VarExp(string pred,string subj,bool val):Subjects are restricted to one or more lower case letters.");*/
    _instance=_universe.size();

    //istringstream sstrm(subj);
    //string tmp="";
    //while(sstrm>>tmp)_subject.push_back(Subject(tmp));

    /*
    for (auto it : subj) _universesubjects.insert(it);
    _universepredicates.insert(_predicate);
    if(_universe.find(make_tuple(_predicate,_subject,val))==_universe.end())
        Assignment(val);
    else
        _universe.insert(make_tuple(_predicate,_subject,val));
    */
    AddTupleToUniverse(_predicate,_subject,val);
}
//TODO: should I require a value? This has a potential issue since no tuple at all will exist in the universe...
VarExp::VarExp (string pred,vector<Subject> subj)
    : BoolExp(VAR_EXP),_predicate(pred),_subject(subj),_arity(_subject.size())
{
    //if (!isupper(pred.c_str()[0])||pred.size()>1)
    if (!isupper(pred.c_str()[0]))
        throw runtime_error("RUNTIME-ERROR:VarExp::VarExp(string pred,string subj,bool val):Predicates must begin with an upper case letter.");
    //throw runtime_error("RUNTIME-ERROR:VarExp::VarExp(string pred,string subj,bool val):Predicates are restricted to upper case single letters.");
    _instance=_universe.size();
}


VarExp::~VarExp ( )
{}
bool VarExp::InUniverse()
{
    string subj = StringifySubjects(_subject);
    auto true_tuple = _universe.find(make_tuple(_predicate,subj,true));
    auto false_tuple = _universe.find(make_tuple(_predicate,subj,false));
    //auto unk_tuple = _universe.find(make_tuple(_predicate,_subject,UNKNOWN));
    if (true_tuple!=_universe.end()) return true;
    else if (false_tuple!=_universe.end()) return true;
    //else if (unk_tuple!=_universe.end()) return true;
    else return false;
}

bool VarExp::Evaluate()
{
    /*
    auto true_tuple = _universe.find(make_tuple(_predicate,_subject,true));
    auto false_tuple = _universe.find(make_tuple(_predicate,_subject,false));
    if (true_tuple!=_universe.end())
    {
    	cout << "DEBUG: true" <<endl;
    	return get<2>(*true_tuple);
    }
    else if (false_tuple!=_universe.end())
    {
    	cout << "DEBUG: false" <<endl;
    	return get<2>(*false_tuple);
    }
    else
    {
    	cout << "DEBUG: unknown" <<endl;
        throw runtime_error("RUNTIME-ERROR:bool VarExp::Evaluate() const: VarExp had unknown true or false value");
    }
    */
    //else return false; //TODO: Default to false, throw, or prompt?
    /*
    else
    {
    	...prompt for value...
    }
    */
    string subj = StringifySubjects(_subject);
    if (_universe.find(make_tuple(_predicate,subj,true))!=_universe.end())
    {
        //cout << "DEBUG:"<<this->Name()<<" is true" <<endl;
        return true;
    }
    else if (_universe.find(make_tuple(_predicate,subj,false))!=_universe.end())
    {
        //cout << "DEBUG:"<<this->Name()<<" is false" <<endl;
        return false;
    }
    else
    {
        //cout << "DEBUG: subject universe : " << SubjectUniverse() << endl;
        //cout << "DEBUG: predicate universe : "<<PredicateUniverse() << endl;
        //cout << "DEBUG: universe :"<<Universe() << endl;
        //throw runtime_error("RUNTIME-ERROR:bool VarExp::Evaluate() const: VarExp had unknown true or false value");
        string usrchoice = "";
        cout <<endl<< "UNIVERSE-ERROR: Not enough information to evaluate. The value of "
             << this->Name() << " is currently unknown"
             << " please choose a true or false valuation (T/F):"
             << endl;
        cin >> usrchoice;
        if (usrchoice=="T"||usrchoice=="t") this->Assignment(true);
        else this->Assignment(false);
        return this->Evaluate();
    }
}
bool VarExp::Evaluate(set<tuple<string,string,bool>> univ,set<string> subj,set<string> pred) const
{
    string cursubj = StringifySubjects(_subject);
    auto findtrue = univ.find(make_tuple(_predicate,cursubj,true));
    auto findfalse = univ.find(make_tuple(_predicate,cursubj,false));
    if (findtrue!=univ.end()&&findfalse==univ.end())
        return true;
    else if (findtrue==univ.end()&&findfalse!=univ.end())
        return false;
    else if (findtrue!=univ.end()&&findfalse!=univ.end())
        throw runtime_error("RUNTIME-ERROR: bool VarExp::Evaluate(set<tuple<string,string,bool>> univ,set<string> subj,set<string> pred) const: "
                            "Universe contains contradiction for relation "+Name());
    else if (findtrue==univ.end()&&findfalse==univ.end())
        throw runtime_error("RUNTIME-ERROR: bool VarExp::Evaluate(set<tuple<string,string,bool>> univ,set<string> subj,set<string> pred) const: "
                            "Universe does not contain relation "+Name());
    else
        throw runtime_error("RUNTIME-ERROR: bool VarExp::Evaluate(set<tuple<string,string,bool>> univ,set<string> subj,set<string> pred) const: "
                            "unknown error on relation "+Name());
}
bool VarExp::Evaluate(vector<tuple<string,string,bool>>univ,vector<string>subj,vector<string>pred) const
{
    string cursubj = StringifySubjects(_subject);
    auto findtrue = find(univ.begin(),univ.end(),make_tuple(_predicate,cursubj,true));
    auto findfalse = find(univ.begin(),univ.end(),make_tuple(_predicate,cursubj,false));
    if (findtrue!=univ.end()&&findfalse==univ.end())
        return true;
    else if (findtrue==univ.end()&&findfalse!=univ.end())
        return false;
    else if (findtrue!=univ.end()&&findfalse!=univ.end())
        throw runtime_error("RUNTIME-ERROR: bool VarExp::Evaluate(vector<tuple<string,string,bool>> univ,vector<string> subj,vector<string> pred) const: "
                            "Universe contains contradiction for relation "+Name());
    else if (findtrue==univ.end()&&findfalse==univ.end())
        throw runtime_error("RUNTIME-ERROR: bool VarExp::Evaluate(vector<tuple<string,string,bool>> univ,vector<string> subj,vector<string> pred) const: "
                            "Universe does not contain relation "+Name());
    else
        throw runtime_error("RUNTIME-ERROR: bool VarExp::Evaluate(vector<tuple<string,string,bool>> univ,vector<string> subj,vector<string> pred) const: "
                            "unknown error on relation "+Name());
}
bool VarExp::TestEvaluate(vector<tuple<string,string,bool>>&univ,vector<string>&subj,vector<string>&pred) const
{
    cout <<"DEBUG:TestEvaluate(): "<< SubjectUniverse() << endl;
    cout <<"DEBUG:TestEvaluate(): "<< Name() << endl;
    string cursubj = StringifySubjects(_subject);
    auto findtrue = find(univ.begin(),univ.end(),make_tuple(_predicate,cursubj,true));
    auto findfalse = find(univ.begin(),univ.end(),make_tuple(_predicate,cursubj,false));
    if (findtrue!=univ.end()&&findfalse==univ.end())
        return true;
    else if (findtrue==univ.end()&&findfalse!=univ.end())
        return false;
    else if (findtrue!=univ.end()&&findfalse!=univ.end())
        throw runtime_error("RUNTIME-ERROR: bool VarExp::TestEvaluate(vector<tuple<string,string,bool>> univ,vector<string> subj,vector<string> pred) const: "
                            "Universe contains contradiction for relation "+Name());
    else if (findtrue==univ.end()&&findfalse==univ.end())
    {
        univ.push_back(make_tuple(_predicate,cursubj,true));
    }
    else
        throw runtime_error("RUNTIME-ERROR: bool VarExp::TestEvaluate(vector<tuple<string,string,bool>> univ,vector<string> subj,vector<string> pred) const: "
                            "unknown error on relation "+Name());
}
shared_ptr<BoolExp> VarExp::NoEvalCopy()
{
    return shared_ptr<BoolExp>(new VarExp(_predicate,_subject));
}
shared_ptr<BoolExp> VarExp::Copy()
{
    bool val = Evaluate();
    return shared_ptr<BoolExp>(new VarExp(_predicate,_subject,val));
}
/*shared_ptr<BoolExp> VarExp::PCopy() const
{
    return shared_ptr<BoolExp>(new VarExp(_predicate,_subject));
}*/
string VarExp::Name() const
{
    //parenthesized notation:
    /*string out = "(";
    for (auto it : _subject)
    {
        out +=it;
        out += ",";
    }
    out[out.size()-1]=')';
    */

    return _predicate+StringifySubjects(_subject);
    //juxtaposed notation:
    //return _predicate+_subject;
}
BoolReturn VarExp::Infer()
{
    return BoolReturn( shared_ptr<BoolExp>(NoEvalCopy()),
                       shared_ptr<BoolExp>(NoEvalCopy()));
}
unsigned VarExp::Arity() const
{
    return _arity;
}
unsigned VarExp::Instance() const
{
    return _instance;
}
string VarExp::Predicate() const
{
    return _predicate;
}
vector<Subject> VarExp::SubjectVector() const
{
    return vector<Subject>(_subject);
}
//TODO: implement that correctly
string VarExp::Subjects() const
{
    return "TODO: IMPLEMENT THIS CORRECTLY";
}
void VarExp::Assignment(bool val)
{
    string subj=StringifySubjects(_subject);
    auto true_tuple = _universe.find(make_tuple(_predicate,subj,true));
    auto false_tuple = _universe.find(make_tuple(_predicate,subj,false));
    //auto unk_tuple = _universe.find(make_tuple(_predicate,_subject,UNKNOWN));
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
    //_universe.insert(make_tuple(_predicate,subj,val));
    AddTupleToUniverse(_predicate,_subject,val);
}
//PHI[src/dst] "replace all occurrences of src with dst in formula PHI"
void VarExp::SwapDependentVar(Subject src,Subject dst)
{
    //cout << "DEBUG: SWAPPING src=" << src << " dst=" << dst << " in " << this->Name()
    //<< " _subject=" << _subject;
    for (unsigned i=0; i<_subject.size(); i++) if (_subject[i].Name() == src.Name()) _subject[i] = dst;
    //cout << " now " << this->Name()<< endl;
}
//TODO: is this safe, do I want this?
void VarExp::SwapDependentVar(Subject src,Subject dst,bool val)
{
    string subj = StringifySubjects(_subject);
    for (unsigned i=0; i<_subject.size(); i++) if (_subject[i].Name() == src.Name()) _subject[i] = dst;
    auto true_tuple = _universe.find(make_tuple(_predicate,subj,true));
    auto false_tuple = _universe.find(make_tuple(_predicate,subj,false));
    //auto unk_tuple = _universe.find(make_tuple(_predicate,_subject,UNKNOWN));
    //if (true_tuple!=_universe.end()||false_tuple!=_universe.end()||unk_tuple!=_universe.end()) return;
    if (true_tuple!=_universe.end()||false_tuple!=_universe.end()) return;
    else _universe.insert(make_tuple(_predicate,subj,val));
}
string VarExp::ReturnDependentVar() const
{
    throw runtime_error("RUNTIME-ERROR:string VarExp::ReturnDependentVar() const: Unimplemented call");
}
//TODO:make this reassign?
void VarExp::AssignChildren(bool val)
{
    AddTupleToUniverse(_predicate,_subject,val);
}
