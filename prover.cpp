#include "prover.h"

//construct the prover with sane defaults
Prover::Prover()
    :_premcount(0), _highestasm(1)
{
    //seed rng
    //srand((unsigned)time(0));
}

//need to fill this in to clean up properly
Prover::~Prover()
{}

//return the index of the negation of the input string in _premi[i]
int Prover::MatchNegation(string input)
{
    int ret = 0;
    string negate = "~"+input;

    for (unsigned int i = 0; i < _premi.size(); i++)
    {
        if (negate == _premi[i]->Name()) {
            ret = i;
        }
    }
    return ret;
}

//return the index of the negation of the input string in _premi[i]
//only if it is not contradicted/blocked
int Prover::MatchNegationUB(string input)
{
    int ret = 0;
    string negate = "~"+input;

    for (unsigned int i = 0; i < _premi.size(); i++)
    {
        if (negate == _premi[i]->Name() && _assum[i] != -1) {
            ret = i;
        }
    }
    return ret;
}

//find the index of the input string
int Prover::MatchString(string input)
{
    int ret = 0;

    for (unsigned int i = 0; i < _premi.size(); i++)
    {
        if (input == _premi[i]->Name()) {
            ret = i;
        }
    }
    return ret;
}

//find the index of the input string
//only if it is not contradicted/blocked
int Prover::MatchStringUB(string input)
{
    int ret = 0;

    for (unsigned int i = 0; i < _premi.size(); i++)
    {
        if (input == _premi[i]->Name() && _assum[i] != -1) {
            ret = i;
        }
    }
    return ret;
}

//setup functions
//add a premise/proposition/inference to the proof
void Prover::AddPremise(shared_ptr<BoolExp> exp, PropType p, string reason)
{
    cout << "DEBUG: attempt to add "<< exp->Name() << endl;
    _blocked.push_back(0);	//every premise begins unblocked and uncontradicted
    _assum.push_back(_highestasm);	//record the highest assumption at the time of creation
    BoolReturn inf = exp->Infer(); //we need to check what this expression contains

    //all VAR_EXP and NOT_EXP containing VAR_EXP's are starred by default
    //starred expressions have nothing useful that can be inferred
    //stars are printed next to each line number
    if (p == VAR_EXP)
    {
        //cout << "DEBUG: adding tuple for VAR_EXP: " << exp->Name()<<endl;
        //exp->AssignTupleInUniverse(exp->Predicate(),exp->SubjectVector(),true);
        //TODO: I think false is fine here not sure though
        exp->AssignTupleInUniverse(exp->Predicate(),exp->SubjectVector(),false);
        _starred.push_back(1);
    }
    else if (p == NOT_EXP && inf.op1->Type() == VAR_EXP)
    {
        //cout << "DEBUG: adding tuple for NOT_EXP: " << exp->Name()<<endl;
        exp->AssignTupleInUniverse(inf.op1->Predicate(),inf.op1->SubjectVector(),false);
        _starred.push_back(1);
    }
    else
    {
        _starred.push_back(0);	//everything else is unstarred
    }
    //TODO: delete this?
    exp->AssignChildren(false);

    ++_premcount;	//increment the count of premises/propositions
    _premi.push_back(exp);	//add the premise/proposition
    _reasons.push_back(reason);	//log the reason/rule justifying it
    _ptypes.push_back(p);	//log the type
    cout << "DEBUG: new universe" << exp->Universe() << endl;
}

//apply an inference rule if possible to the formula at index i
//there are special rules for different kinds of NOT_EXP's hence why the NOT_EXP
//branch has a further nested switch statement
void Prover::Infer(int i)
{
    //do not infer anything from blocked/contradicted expressions
    if (_blocked[i]) return;

    switch(_ptypes[i])
    {
    case OR_EXP:
    {
        OrInference(i);
        break;
    }
    case VAR_EXP:
        break;
    case AND_EXP:
    {
        AndInference(i);
        break;
    }
    case NOT_EXP:
    {
        //we need to switch on the type contained in the not expression
        BoolReturn inf = _premi[i]->Infer();

        switch(inf.op1->Type())
        {
        case NOT_EXP:
        {
            NotNotInference(i);
            break;
        }
        case COND_EXP:
        {
            NotCondInference(i);
            break;
        }
        case OR_EXP:
        {
            NotOrInference(i);
            break;
        }
        case AND_EXP:
        {
            NotAndInference(i);
            break;
        }
        case UNIV_EXP:
        {
            //throw runtime_error("RUNTIME-ERROR:void Prover::Infer(int i): Not universal infer not implemented yet");
            cout << "DEBUG: NOT UNIV INFER " << endl;
            NotUnivInference(i);
            break;
        }
        case EXIST_EXP:
        {
            cout << "DEBUG: NOT EXIST INFER " << endl;
            //throw runtime_error("RUNTIME-ERROR:void Prover::Infer(int i): Not existential infer not implemented yet");
            NotExistInference(i);
            break;
        }
        default:
        {
            break;
        }
        }
        break;
    }
    case COND_EXP:
    {
        CondInference(i);
        break;
    }
    case UNIV_EXP:
    {
        //cout << "DEBUG: UNIV EXPRESSION!" << endl;
        cout << "DEBUG: UNIV INFER " << endl;
        UnivInference(i);
        break;
    }
    case EXIST_EXP:
    {
        cout << "DEBUG: EXIST INFER " << endl;
        //throw runtime_error("RUNTIME-ERROR:void Prover::Infer(int i): existential infer not implemented yet");
        //cout << "DEBUG: EXIST EXPRESSION!" << endl;
        ExistInference(i);
        break;
    }
    default:
    {
        break;
    }
    }
}

//if possible make an assumption
bool Prover::MakeAssumption()
{
    bool ret = false;

    //find only one assumption if possible
    for (int i = 0; i < _premcount && !ret; i++)
    {
        if (NotAndAsm(i)||OrAsm(i)||CondAsm(i))
        {
            ret = true;
        }
    }

    return ret;
}

//search for any contradictions
//notify the user if one is found
//block off any contradictions at the proper assumption levels
bool Prover::FindContradiction()
{
    //use match negation, check each formula for its own negation
    bool ret = false;
    int contraa = -1;
    int contrab = -1;

    //check for a contradiction and notify the user
    for (unsigned int i = 0; i < _premi.size(); i++)
    {
        if (MatchNegationUB(_premi[i]->Name())&&_assum[i]!=-1)
        {
            cout << "Negation of line " << i << " found!" << endl;
            cout << "line " << i << " contradicts line " << MatchNegationUB(_premi[i]->Name()) << endl;
            ret = true;
            //whenever a contradiction is found a whole set of assumptions is invalidated
            _highestasm--;
            contraa = i;
            contrab = MatchNegationUB(_premi[i]->Name());
            break;
        }
    }

    //add the new premise by RAA and block off all lines at the previous assumption level
    if (ret == true)
    {
        int k = 0;

        //find the premise to negate
        while(_assum[k]!=_highestasm+1) k++;
        shared_ptr<BoolExp> tmp(new NotExp(_premi[k]->NoEvalCopy()));

        //add it to the proof if possible
        if(_highestasm) AddPremise(tmp,tmp->Type(), "by contradiction "+to_string(contraa)+", "+to_string(contrab));
        //else delete tmp;

        //block off the relevant propositions
        for (unsigned int i = 0; i < _premi.size(); i++)
        {
            if (_assum[i] ==_highestasm+1||_blocked[i])
            {
                _blocked[i]++;
                _assum[i] = -1;
            }
        }
    }
    return ret;
}

//pretty print the list of premises and reasons this will mind the current starring
//and level of blocking
void Prover::PrintPremises() const
{
    cout << "PREMISES: " << endl;
    int j = 0;

    for (auto i = _premi.begin(); i != _premi.end(); ++i, ++j)
    {
        if (_starred[j])
        {
            cout << "* ";
        }
        else
        {
            cout << "  ";
        }

        cout << j << ") ";

        if(_blocked[j] > 0)
        {
            for (int k = _blocked[j]; k != 0; k--)
            {
                cout << "[";
            }
        }

        cout << " " << _premi[j]->Name();
        cout << "\t{ " + _reasons[j] + " }" << endl;
    }
}

//some useful methods
string Prover::PremiseName(int i) const
{
    return _premi[i]->Name();
}
//TODO: does univ need an &?
bool Prover::Evaluate(int i, vector<tuple<string,string,bool>> univ,vector<string> subj,vector<string> pred) const
{
    return _premi[i]->Evaluate(univ,subj,pred);
}
bool Prover::TestEvaluate(int i, vector<tuple<string,string,bool>>&univ,vector<string>&subj,vector<string>&pred) const
{
    return _premi[i]->TestEvaluate(univ,subj,pred);
}
int Prover::PremiseCount() const
{
    return _premcount;
}
int Prover::HighestAssumption() const
{
    return _highestasm;
}

//inference rules
void Prover::OrInference(int i)
{
    //RULE: (AvB), ~A |- B
    BoolReturn inf = _premi[i]->Infer();

    //search for the negation of the first operand
    if (MatchNegationUB(inf.op1->Name()))
    {
        //if not already in the proof add the second operand
        if (!MatchStringUB(inf.op2->Name()))
        {
            _starred[i] = 1;
            AddPremise(inf.op2->NoEvalCopy(),inf.op2->Type(),
                       "(AvB), ~A |- B on "+to_string(i)+", "+to_string(MatchNegationUB(inf.op1->Name())));
        }
    }
    //again but with the second operand
    if (MatchNegationUB(inf.op2->Name()))
    {
        if (!MatchStringUB(inf.op1->Name()))
        {
            _starred[i] = 1;
            AddPremise(inf.op1->NoEvalCopy(),inf.op1->Type(),
                       "(AvB), ~A |- B on " + to_string(i) + ", "+to_string(MatchNegationUB(inf.op2->Name())));
        }
    }

}
void Prover::AndInference(int i)
{
    //RULE: (A&B) |- A, B
    BoolReturn inf = _premi[i]->Infer();

    //add the first operand if it is not already present in the proof
    if (!MatchStringUB(inf.op1->Name()))
    {
        _starred[i] = 1;
        AddPremise(inf.op1->NoEvalCopy(),inf.op1->Type(),
                   "(A&B) |- A, B on "+to_string(i));
    }
    if (!MatchStringUB(inf.op2->Name()))
    {
        _starred[i] = 1;
        AddPremise(inf.op2->NoEvalCopy(),inf.op2->Type(),
                   "(A&B) |- A, B on "+to_string(i));
    }

}
void Prover::CondInference(int i)
{
    //RULE: (A->B), A |- B
    BoolReturn inf = _premi[i]->Infer();

    //check if the first operand already exists in the proof
    if (MatchStringUB(inf.op1->Name()))
    {
        //check if the second operand already exists in the proof
        //if not add it
        if (!MatchStringUB(inf.op2->Name()))
        {
            _starred[i] = 1;
            AddPremise(inf.op2->NoEvalCopy(),inf.op2->Type(),
                       "(A->B), A |- B on " +to_string(i)+", "+to_string(MatchStringUB(inf.op1->Name())));
        }
    }

    //RULE: (A->B), ~B |- ~A
    //check if the negation of the secon operand already exists in the proof
    if (MatchNegationUB(inf.op2->Name()))
    {
        //check if the negation of the first operand already exists in the proof
        //if not add it
        if (!MatchNegationUB(inf.op1->Name()))
        {
            shared_ptr<BoolExp> tmp(new NotExp(inf.op1->NoEvalCopy()));
            _starred[i] = 1;
            AddPremise(tmp,tmp->Type(),
                       "(A->B), ~B |- ~A on "+to_string(i)+", "+to_string(MatchNegationUB(inf.op2->Name())));
        }
    }

}

void Prover::NotNotInference(int i)
{
    //RULE: ~~A |- A
    BoolReturn inf = _premi[i]->Infer();
    inf = inf.op1->Infer();

    //if the unnegated proposition does not already exist in the proof add it
    if (!MatchStringUB(inf.op1->Name()))
    {
        _starred[i] = 1;
        AddPremise(inf.op1->NoEvalCopy(),inf.op1->Type(),
                   "~~A |- A on " + to_string(i));
    }
}
void Prover::NotOrInference(int i)
{
    //~(AvB) |- ~A, ~B
    BoolReturn inf = _premi[i]->Infer();
    inf = inf.op1->Infer();

    //check if both negated operands already exist in the proof
    //if they do not then add them
    if (!MatchNegationUB(inf.op1->Name()))
    {
        _starred[i] = 1;
        shared_ptr<BoolExp> tmp(new NotExp(inf.op1->NoEvalCopy()));
        AddPremise(tmp,tmp->Type(),
                   "~(AvB) |- ~A, ~B on " +to_string(i));
    }

    if (!MatchNegationUB(inf.op2->Name()))
    {
        _starred[i] = 1;
        shared_ptr<BoolExp> tmp(new NotExp(inf.op2->NoEvalCopy()));
        AddPremise(tmp,tmp->Type(),
                   "~(AvB) |- ~A, ~B on " +to_string(i));
    }

}
void Prover::NotAndInference(int i)
{
    //~(A&B), A |- ~B
    BoolReturn inf = _premi[i]->Infer();
    inf = inf.op1->Infer();

    //check if the first operand exists in the proof
    if (MatchStringUB(inf.op1->Name()))
    {
        //check if the negation of the second operand exists in the proof
        //if not add it to the proof
        if (!MatchNegationUB(inf.op2->Name()))
        {
            shared_ptr<BoolExp> tmp(new NotExp(inf.op2->NoEvalCopy()));
            _starred[i] = 1;
            AddPremise(tmp,tmp->Type(),
                       "~(A&B), A |- ~B on "+to_string(i)+", "+to_string(MatchStringUB(inf.op1->Name())));
        }
    }
    //same idea but operand 2
    if (MatchStringUB(inf.op2->Name()))
    {
        if (!MatchNegationUB(inf.op1->Name()))
        {
            shared_ptr<BoolExp> tmp(new NotExp(inf.op1->NoEvalCopy()));
            _starred[i] = 1;
            AddPremise(tmp,tmp->Type(),
                       "~(A&B), A |- ~B on " + to_string(i) +", "+to_string(MatchStringUB(inf.op2->Name())));

        }
    }
}
void Prover::NotCondInference(int i)
{
    //~(A->B) |- A, ~B
    BoolReturn inf = _premi[i]->Infer();
    inf = inf.op1->Infer();

    //check to see if the propositions already exist before adding them
    if (!MatchStringUB(inf.op1->Name())||!MatchNegationUB(inf.op2->Name()))
    {
        shared_ptr<BoolExp> tmp(new NotExp(inf.op2->NoEvalCopy()));
        _starred[i] = 1;
        AddPremise(inf.op1->NoEvalCopy(),inf.op1->Type(),
                   "~(A->B) |- A, ~B on "+to_string(i));
        AddPremise(tmp,tmp->Type(),
                   "~(A->B) |- A, ~B on "+to_string(i));
    }

}

//methods for making assumptions
bool Prover::NotAndAsm(int i)
{
    bool ret = false;
    BoolReturn inf = _premi[0]->Infer();
    inf = _premi[i]->Infer();

    if(_premi[i]->Type() == NOT_EXP && _premi[i]->Infer().op1->Type() == AND_EXP && //if a nand expression is found
            (!MatchStringUB( _premi[i]->Infer().op1->Infer().op1->Name()  )			//and the lhs&rhs are not already present
             &&!MatchStringUB( _premi[i]->Infer().op1->Infer().op2->Name()  )))
    {
        if (!MatchStringUB(inf.op1->Infer().op1->Name())			//if the lhs is not present
                &&!MatchNegationUB(inf.op1->Infer().op2->Name())	//and the negation of the lhs and rhs
                &&!MatchNegationUB(inf.op1->Infer().op1->Name()))	//are not already present
        {
            ++_highestasm;											//increment the highest assumption count
            AddPremise(inf.op1->Infer().op1->NoEvalCopy(),				//add the lhs as an assumption
                       inf.op1->Infer().op1->Type(),
                       "negated and assumption to break "+ to_string(i));
            ret = true;
        }

        if (!MatchStringUB(inf.op1->Infer().op2->Name())			//if the rhs is not present
                &&!MatchNegationUB(inf.op1->Infer().op1->Name())	//and the negation of the rhs and lhs
                &&!MatchNegationUB(inf.op1->Infer().op2->Name()) )	//are not already present
        {
            ++_highestasm;											//increment the assumption count
            AddPremise(inf.op1->Infer().op2->NoEvalCopy(),				//add the rhs as an assumption
                       inf.op1->Infer().op2->Type(),
                       "negated and assumption to break "+ to_string(i));
            ret = true;
        }

    }
    return ret;
}
bool Prover::OrAsm(int i)
{
    bool ret = false;
    BoolReturn inf = _premi[0]->Infer();
    inf = _premi[i]->Infer();

    //if _premi[i] is an OR_EXP and the negation of either operands are not already present
    if(_premi[i]->Type() == OR_EXP &&
            (!MatchNegationUB(inf.op1->Name())&&!MatchNegationUB(inf.op2->Name())))
    {
        if (!MatchStringUB(inf.op1->Name()))	//add the lhs if it is not already present
        {
            ++_highestasm;
            AddPremise(inf.op1->NoEvalCopy(),inf.op1->Type(), "or assumption to break " + to_string(i));
            ret = true;
        }

        if (!MatchStringUB(inf.op2->Name()))	//add the rhs if it is not already present
        {
            ++_highestasm;
            AddPremise(inf.op2->NoEvalCopy(),inf.op2->Type(), "or assumption to break " + to_string(i));
            ret = true;
        }
    }

    return ret;
}
bool Prover::CondAsm(int i)
{
    bool ret = false;
    BoolReturn inf = _premi[0]->Infer();
    inf = _premi[i]->Infer();

    //if _premi[i] is a conditional and
    //either the lhs is not present or the negation of the rhs is not present
    if(_premi[i]->Type() == COND_EXP
            &&!MatchStringUB(inf.op1->Name())
            &&!MatchNegationUB(inf.op2->Name()))
    {
        if (!MatchStringUB(inf.op1->Name()))
        {
            ++_highestasm;
            AddPremise(inf.op1->NoEvalCopy(),inf.op1->Type(), "conditional assumption to break " + to_string(i));
            ret = true;
        }
    }
    return ret;
}
/*
void Prover::AssignByFormula(Context& context)
{
    vector<string> assigned;
    vector<string> unassigned;
    bool trf = false;

    //for every formula in the proof
    for (int i = 0; i < _premcount; i++ )
    {
        //get a variable letter from the current formula
        Traverse(unassigned,_premi[i]);

        //assign to every unassigned variable that was found
        for (unsigned int i = 0; i < unassigned.size(); i++)
        {
            if (find(assigned.begin(),assigned.end(),unassigned[i]) == assigned.end())
            {
                cout << unassigned[i] << " = " << endl;
                cin >> trf;
                cin.ignore(256,'\n');
                context.AssignByStr(unassigned[i],trf);
                assigned.push_back(unassigned[i]);
            }
        }

        //clear the unassigned vector in prep for the next line
        unassigned.clear();
    }
}*/
void Prover::Traverse(vector<string>& vec, shared_ptr<BoolExp> boolexp)
{
    if (boolexp->Type() != VAR_EXP)
    {
        //cout << "debug boop"<<endl;
        if (boolexp->Infer().op1->Type() == VAR_EXP)
        {
            vec.push_back(boolexp->Infer().op1->Name());
        }
        else
        {
            Traverse(vec,boolexp->Infer().op1);
        }

        if (boolexp->Infer().op2->Type() == VAR_EXP)
        {
            vec.push_back(boolexp->Infer().op2->Name());
        }
        else
        {
            Traverse(vec,boolexp->Infer().op2);
        }
    }
}
/****************************Quantifier inferences*************************************/
//TODO: work more on infinite loop problem
//IDEA: Maybe only add new subjects based on the first loop else try to add based on old subjects
void Prover::UnivInference(int i)
{
    //don't infer using new subjects if unstarred unblocked formulas that are not universals are left to do
    //ignore unstarred nands, ors and conditionals since those may require assumptions to break
    //for (int j = 0; j < _premi.size(); j++)
    //{
    //    if (_starred[j]!=1&&_premi[j]->Type()!=UNIV_EXP)
    //        //if (_starred[j]!=1&&_premi[j]->Type()!=UNIV_EXP&&_premi[j]->Type()!=OR_EXP)
    //        //if (_starred[j]!=1&&_premi[j]->Type()!=UNIV_EXP&&_premi[j]->Type()!=OR_EXP&&_premi[j]->Type()!=COND_EXP)
    //    {
    //        cout << "DEBUG: cannot univ infer because " <<_premi[j]->Name()<< endl;
    //        return;
    //    }
    //}
    cout <<endl<< "DEBUG: univ()" << endl;
    cout << "DEBUG: universe: "<< _premi[i]->SubjectUniverse() << endl;
    PrintPremises();

    //pick a random univ expression
    vector<int> univIndices;
    unsigned randIndex = 0;
    for (int j = 0; j < _premi.size(); j++)
        if (_premi[j]->Type()==UNIV_EXP)
        {
            univIndices.push_back(j);
        }

    cout << "DEBUG: univIndices=";
    for ( auto it : univIndices) cout << it << " ";
    cout << endl;

    randIndex = rand()%univIndices.size();
    i = univIndices[randIndex];
    cout << "DEBUG: randIndex=" <<randIndex<< endl;
    cout << "DEBUG: univIndices.size()=" <<univIndices.size()<< endl;

    //RULE: (All x)Phi(x) |- Phi(a) for any constant a
    BoolReturn inf = _premi[i]->Infer();
    set<string> subjects = inf.op1->UniverseSubjectsCopy();
    string subj = _premi[i]->ReturnDependentVar();

    //check each relation with each constant and add a child formula with one of the nonmatched predicate if possible
    for (auto it : subjects)
    {
        inf.op1->SwapDependentVar(subj,it);
        cout << "DEBUG: swap a "<<inf.op1->Name() <<endl;
        cout << "DEBUG: univ infer checking name " << inf.op1->Name() << endl;
        if (!MatchStringUB(inf.op1->Name()))
        {
            cout << "DEBUG: add premise " << inf.op1->Name() << endl;
            AddPremise(inf.op1->NoEvalCopy(),inf.op1->Type(),"(All x)Phi(x) |- Phi(a) on line "+to_string(i));
            cout << "DEBUG END UNIV" <<endl<<endl;
            return;
        }
        inf.op1->SwapDependentVar(it,subj);
        cout << "DEBUG: swap b "<<inf.op1->Name() <<endl;
    }

    //don't infer using new subjects if unstarred unblocked formulas that are not universals are left to do
    //ignore unstarred nands, ors and conditionals since those may require assumptions to break
    for (int j = 0; j < _premi.size(); j++)
    {
        if (_starred[j]!=1&&_premi[j]->Type()!=UNIV_EXP)
            //if (_starred[j]!=1&&_premi[j]->Type()!=UNIV_EXP&&_premi[j]->Type()!=OR_EXP)
            //if (_starred[j]!=1&&_premi[j]->Type()!=UNIV_EXP&&_premi[j]->Type()!=OR_EXP&&_premi[j]->Type()!=COND_EXP)
        {
            cout << "DEBUG: cannot univ infer because " <<_premi[j]->Name()<< endl;
            return;
        }
    }


    //if not find some arbitrary new constant to use
    cout << "DEBUG: find new arbitrary constant " <<endl;
    for(unsigned j = 1;; j++)
    {
        inf.op1->SwapDependentVar(subj,"a_"+to_string(j));
        PrintPremises();
        cout << "DEBUG: :] randIndex=" <<randIndex<< endl;
        cout << "DEBUG: swap c "<<inf.op1->Name() <<endl;
        if (!MatchStringUB(inf.op1->Name()))
        {
            AddPremise(inf.op1->NoEvalCopy(),inf.op1->Type(),"(All x)Phi(x) |- Phi(a) for a new constant a, on "+to_string(i));
            cout << "DEBUG END UNIV" <<endl<<endl;
            return;
        }
        inf.op1->SwapDependentVar("a_"+to_string(j),subj);
        cout << "DEBUG: swap d "<<inf.op1->Name() <<endl;
    }
    throw runtime_error("RUNTIME-ERROR:void Prover::UnivInference(int i):Should not reach this point");
    cout << "DEBUG END UNIV" <<endl<<endl;
}

void Prover::ExistInference(int i)
{
    cout << "DEBUG: exist()" << endl;
    if (_starred[i]==1) return;
    //RULE: (Some x)Phi(x) |- Phi(a) for a new constant a
    BoolReturn inf = _premi[i]->Infer();
    //set<string> subjects = inf.op1->UniverseSubjectsCopy();
    cout << "DEBUG: bing"<<endl;
    set<string> subjects(inf.op1->UniverseSubjectsCopy());
    //set<string> subjects(inf.op1->UniverseSubjectsCopy().begin(),inf.op1->UniverseSubjectsCopy().end());
    cout << "DEBUG: bang"<<endl;
    string subj = _premi[i]->ReturnDependentVar();
    unsigned postfix = 1;

    cout << "DEBUG: subjects={ ";
    for (auto it : subjects) cout <<" "<< it;
    cout <<" }"<< endl;
    cout << "DEBUG: inf.op1->UniverseSubjectsCopy()={";
    for (auto it : inf.op1->UniverseSubjectsCopy()) cout <<" "<< it;
    cout << " }" <<endl;

    //find a new constant if subjects is nonempty
    for (auto it : subjects)
    {
        inf.op1->SwapDependentVar(subj,"b_"+to_string(postfix));
        cout << "DEBUG: compare " << subj << " to " << "b_" << postfix << endl;
        if ("x_"+to_string(postfix)==it||MatchString(inf.op1->Name()))
        {
            cout << "DEBUG: Finding new constant for existential instantiation..." << endl;
            inf.op1->SwapDependentVar("b_"+to_string(postfix),subj);
            postfix++;
            //inf.op1->SwapDependentVar("x_"+to_string(postfix),subj);
        }
        else
        {
            //inf.op1->SwapDependentVar(subj,"x_"+to_string(postfix));
            //inf.op1->SwapDependentVar(subj,"x_"+to_string(postfix),true);
            //assign all children varexps to true for now, the only time we use them is when
            //we are checking every combo for a countermodel so this won't harm anything
            AddPremise(inf.op1->NoEvalCopy(),inf.op1->Type(),"(Some x)Phi(x) |- Phi(a) for a new constant a, on "+to_string(i));
            _starred[i] = 1;
            return;
        }
    }
    //if subjects is empty then just add anything
    if (subjects.empty())
    {
        cout << "DEBUG: subjects.empty()=" << subjects.empty() << endl;
        inf.op1->SwapDependentVar(subj,"c_"+to_string(postfix));
        cout << "DEBUG: adding " << inf.op1->Name() << endl;
        AddPremise(inf.op1->NoEvalCopy(),inf.op1->Type(),"(Some x)Phi(x) |- Phi(a) for a new constant a, on "+to_string(i));
        _starred[i] = 1;
        return;
    }
    cout << "DEBUG: dying with subjects:"<< _premi[i]->SubjectUniverse() << endl;
    throw runtime_error("RUNTIME-ERROR:void Prover::ExistInference(int i):Should not reach this point");
}

void Prover::NotExistInference(int i)
{
    cout << "DEBUG: notexist()" << endl;
    //RULE: ~(Some x)Phi(x) |- (All x)~Phi(x)
    BoolReturn inf = _premi[i]->Infer();
    shared_ptr<BoolExp> negatedChild(new NotExp(inf.op1->Infer().op1->NoEvalCopy()));
    //shared_ptr<BoolExp> negatedChild(new NotExp(inf.op1));
    shared_ptr<BoolExp> newExpr(new UniversalExp(negatedChild->NoEvalCopy(),_premi[i]->ReturnDependentVar()));
    //cout << "DEBUG: adding " << newExpr->Name()<<endl;
    if (!MatchStringUB(newExpr->Name()))	//add the formula if it is not already present
    {
        cout << "DEBUG: adding "<< newExpr->Name() << endl;
        AddPremise(newExpr->NoEvalCopy(),newExpr->Type(),"~(Some x)Phi(x) |- (All x)~Phi(x) on "+to_string(i));
    }
    _starred[i] = 1;
}

void Prover::NotUnivInference(int i)
{
    cout << "DEBUG: notuniv()" << endl;
    //RULE: ~(All x)Phi(x) |- (Some x)~Phi(x)
    BoolReturn inf = _premi[i]->Infer();
    shared_ptr<BoolExp> negatedChild(new NotExp(inf.op1->Infer().op1->NoEvalCopy()));
    //shared_ptr<BoolExp> negatedChild(new NotExp(inf.op1));
    shared_ptr<BoolExp> newExpr(new ExistentialExp(negatedChild->NoEvalCopy(),_premi[i]->ReturnDependentVar()));
    //cout << "DEBUG: adding " << newExpr->Name()<<endl;
    if (!MatchStringUB(newExpr->Name()))	//add the formula if it is not already present
    {
        AddPremise(newExpr->NoEvalCopy(),newExpr->Type(),"~(Some x)Phi(x) |- (All x)~Phi(x) on "+to_string(i));
    }
    _starred[i] = 1;
}
