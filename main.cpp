#include "defs.h"
#include "prover.h"
#include <math.h>
#include <algorithm>
#include <unistd.h>
#include <limits>

using namespace std;

vector<Subject> BuildSubjectVector(string str, const char c);
string StripWhitespace(string str);
shared_ptr<BoolExp> BuildExpression(shared_ptr<BoolExp>,istream&,ostream&);
bool findCounterModel(set<tuple<string,string,bool>>,set<string>,set<string>,Prover&,int,int,set<tuple<string,string,bool>>&);
void calculate();
void prove();
int main()
{
    string choice = "";
    cout << "Would you like to use calculate or prove mode(c/p):" <<endl;
    getline(cin,choice);
    if (choice[0] == 'c')
        calculate();
    else if (choice[0]=='p')
        prove();
    else
        throw runtime_error("RUNTIME-ERROR: main(): only calc mode and proof mode are implemented. (p/c)");
    return 0;
}
void calculate()
{
    vector<shared_ptr<BoolExp>> ptr;
    shared_ptr<BoolExp> current = nullptr;
    string input = "c";
    cout << "Building argument enter q to end:" << endl;
    while (input[0] == 'c')
    {
        ptr.push_back(BuildExpression(current,cin,cout));
        cout << endl;
        cout << "Building argument enter q to end or c to continue:" << endl;
        getline(cin,input);
    }
    cout << "pre-eval subjects: " << ptr[0]->SubjectUniverse() << endl;
    cout << "pre-eval predicates: "<<ptr[0]->PredicateUniverse() << endl;
    cout << "pre-eval universe: "<<ptr[0]->Universe() << endl;

    for (auto p : ptr)p->Evaluate(); //eval the list once to get a value for every relation
    for (auto p : ptr)p->Evaluate(); //eval the list a second time to fill in all the quantified relations
    cout << endl;
    cout << "post-eval subjects: " << ptr[0]->SubjectUniverse() << endl;
    cout << "post-eval predicates: "<<ptr[0]->PredicateUniverse() << endl;
    cout << "post-eval universe: "<<ptr[0]->Universe() << endl;
    for (auto p : ptr) cout << p->Name() << "=" << p->Evaluate() << endl; //print the results
}
void prove()
{
    srand((unsigned)time(0)); //seed rng for the prover
    Prover proof;
    vector<shared_ptr<BoolExp>> ptr;
    shared_ptr<BoolExp> current = nullptr;
    string input = "c";
    int premisecount = 0;
    int conclusionindex = 0;
    bool countermodelfound = false;
    set<tuple<string,string,bool>> countermodel;
    cout << "Enter a premise or press q to enter a conclusion:" << endl;
    while (input[0] == 'c')
    {
        current=BuildExpression(current,cin,cout);
        proof.AddPremise(current,current->Type(),"given");
        cout << endl;
        cout << "Press c to enter another premise or press q to enter a conclusion:" << endl;
        getline(cin,input);
        premisecount++;
        conclusionindex++;
    }
    shared_ptr<BoolExp> conclusion = shared_ptr<BoolExp>(new NotExp(BuildExpression(current,cin,cout)));
    proof.AddPremise(conclusion,conclusion->Type(),"~ of the conclusion");
    cout << endl;
    cout << "subjects before proof: " << current->SubjectUniverse() << endl;
    cout << "predicates before proof: "<<current->PredicateUniverse() << endl;
    cout << "universe before proof: "<<current->Universe() << endl;
    cout << "prover before proof:"<<endl;
    cout << "beginning inference step enter q to quit:" << endl;
    proof.PrintPremises();
    input = "c";
    for (int i = 0; input != "q"; i++)
    {
        cout << "Iteration: " << i << endl;
        proof.FindContradiction();
        cout << "Continue inferences or quit?(c/q)" << endl;
        getline(cin,input);
        //every 50 iterations but not the very first one the proof is
        //allowed to try and make an assumption
        if (!(i%50)&&i!=0) {
            proof.MakeAssumption();
        }
        //i%proof.PremiseCount() ensures we never index into an invalid line
        proof.Infer(i%proof.PremiseCount());
        //print status info to the user
        cout << "Assumptions: " << proof.HighestAssumption() << endl;
        cout << "Premises: "<< proof.PremiseCount() << endl;
        //check if a contradiction exists
        proof.FindContradiction();

        //every 100 iterations but not the first the program is allowed
        //to try and find a counterargument
        if(!(i%100)&&i!=0)
        {
            proof.PrintPremises();
            countermodelfound = findCounterModel(conclusion->UniverseCopy(),
                                                 conclusion->UniverseSubjectsCopy(),
                                                 conclusion->UniversePredicatesCopy(),
                                                 proof,
                                                 premisecount,
                                                 conclusionindex,
                                                 countermodel);
        }
        if (proof.HighestAssumption()==0||countermodelfound)
        {
            break;
        }
    }

    //the output differs based on validity
    if(!countermodelfound)
    {
        proof.PrintPremises();
        cout << "Therefore " << conclusion->Infer().op1->Name()<< " is a valid conclusion." << endl;
        cout << "Program Ending" << endl;
        cout << "post-proof subjects: " << conclusion->SubjectUniverse() << endl;
        cout << "post-proof predicates: "<<conclusion->PredicateUniverse() << endl;
        cout << "post-proof universe: "<<conclusion->Universe() << endl;
    }
    else
    {
        proof.PrintPremises();
        //cout << "counter-model: {";
        //for (auto it : countermodel)
        //cout << " ("<<get<0>(it)<<","<<get<1>(it)<<","<<get<2>(it)<<") ";
        //cout <<"}"<< endl;

        cout << "The conclusion is invalid, because the above counterargument can make all"
             << " the premises true \nand the negation of the conclusion true too, which makes"
             << " the intended conclusion false."
             << endl;
    }
}

shared_ptr<BoolExp> BuildExpression(shared_ptr<BoolExp> current,istream& in, ostream& out)
{
    string input = ""; //for user input

    //the only operators supported are unary or binary so ask which one the user wants
    do
    {
        out << "Unary or Binary?(1/2): " << endl;
        getline(in,input);
    }
    while (input[0] != '1' && input[0] != '2');

    //if they want a unary operator it can be either a variable or
    //it can be a negation operator
    if(input[0] == '1')
    {
        do
        {
            out << "Unary variable, negation, universal or existential?(v/n/u/e)" << endl;
            getline(in,input);
        }
        while (input[0] != 'v' && input[0] != 'n' && input[0] != 'u' && input[0] != 'e');

        //if they want a variable we need to know its name
        if (input[0] == 'v')
        {
            string relation = "";
            out << "Enter a relation: " << endl;
            getline(in,relation);
            relation = StripWhitespace(relation);

            string subject_string = "";
            out << "Enter a comma separated list of variable names using the appropriate syntax:" << endl;
            getline(in,subject_string);
            vector<Subject> vec = BuildSubjectVector(subject_string,',');
            current = shared_ptr<BoolExp>(new VarExp(relation,vec));
            return current;
        }

        //if they want a negation operator then we need to recurse to
        //define its operands
        if (input[0] == 'n')
        {
            out << "Continuing to interior of negation: " << endl;
            shared_ptr<BoolExp> temp;
            temp = BuildExpression(current,cin,cout);
            current = shared_ptr<BoolExp>(new NotExp(temp));
            return current;
        }

        //if they want a universal quantifier then we need to recurse to
        //define its operands
        if (input[0] == 'u')
        {
            string var = "";
            out << "Enter the variable to quantify over: " << endl;
            getline(in,var);
            Subject subj(var);

            out << "Continuing to interior of universal: " << endl;
            shared_ptr<BoolExp> temp;
            temp = BuildExpression(current,cin,cout);
            current = shared_ptr<BoolExp>(new UniversalExp(temp,subj));
            return current;
        }
        //if they want a universal quantifier then we need to recurse to
        //define its operands
        if (input[0] == 'e')
        {
            string var = "";
            out << "Enter the variable to quantify over: " << endl;
            getline(in,var);
            Subject subj(var);

            out << "Continuing to interior of existential: " << endl;
            shared_ptr<BoolExp> temp;
            temp = BuildExpression(current,cin,cout);
            current = shared_ptr<BoolExp>(new ExistentialExp(temp,subj));
            return current;
        }

    }

    //binary operators can be and/or/conditionals each one requires recursing
    //to fill in the individual operands
    if (input[0] == '2')
    {
        do
        {
            out << "And, Or, or Conditional?(a/o/c)" << endl;
            getline(in,input);
        }
        while (input[0] != 'a' && input[0] != 'o' && input[0] != 'c');

        if (input[0] == 'a')
        {
            shared_ptr<BoolExp> temp1;
            shared_ptr<BoolExp> temp2;
            out << "Building left side of And: " << endl;
            temp1 = BuildExpression(current,cin,cout);
            out << "Building right side of And: " << endl;
            temp2 = BuildExpression(current,cin,cout);
            current = shared_ptr<BoolExp>(new AndExp(temp1,temp2));
            return current;
        }

        if (input[0] == 'o')
        {
            shared_ptr<BoolExp> temp1;
            shared_ptr<BoolExp> temp2;
            out << "Building left side of Or: " << endl;
            temp1 = BuildExpression(current,cin,cout);
            out << "Building right side of Or: " << endl;
            temp2 = BuildExpression(current,cin,cout);
            current = shared_ptr<BoolExp>(new OrExp(temp1,temp2));
            return current;
        }

        if (input[0] == 'c')
        {
            shared_ptr<BoolExp> temp1;
            shared_ptr<BoolExp> temp2;
            out << "Building left side of Conditional: " << endl;
            temp1 = BuildExpression(current,cin,cout);
            out << "Building right side of Conditional: " << endl;
            temp2 = BuildExpression(current,cin,cout);
            current = shared_ptr<BoolExp>(new CondExp(temp1,temp2));
            return current;
        }
    }
    return nullptr;
}
//from a cplusplus.com post
string StripWhitespace(string str)
{
    std::string str_no_ws ;
    for( char c : str ) if( !std::isspace(c) ) str_no_ws += c ;
    return str_no_ws ;
}
vector<Subject> BuildSubjectVector(string str, const char c)
{
    stringstream in(str);
    vector<Subject> out;
    string temp = "";
    while (getline(in,temp,','))
    {
        out.push_back(Subject(temp));
    }
    return out;
}
//test every proposition in prover with every permutation of 1's and 0's in myvec if every proposition
//ends up true for a given permutation then we have found a counterargument
bool findCounterModel(set<tuple<string,string,bool>> universeset,set<string> subjset,set<string> predset,Prover& proof,int premises,int conclusion,set<tuple<string,string,bool>>& countermodel)
{
    cout << "Checking if the argument is invalid" << endl;
    bool premchk = true; //to switch on the return value
    bool concchk = true; //same
    bool counterarg = false; //the return value is false by default
    vector<tuple<string,string,bool>> universevec(universeset.begin(),universeset.end());
    vector<string> subjvec(subjset.begin(),subjset.end());
    vector<string> predvec(predset.begin(),predset.end());
    vector<bool> myvec;
    myvec.resize(universevec.size(),0);
    sort(universevec.begin(),universevec.end());

    //We need to collect all the subjects in advance
    //proof.CollectSubjects(subjvec);
    //make sure the universe being tested has all the necessary tuples
    cout << "DEBUG: adding missing tuples to universe" << endl;
    for(int i=0; i < premises; i++)
    {
        cout << "DEBUG: test checking " << proof.PremiseName(i) << endl;
        if (!proof.TestEvaluate(i,universevec,subjvec,predvec)) {
            premchk = false;
        }
    }

    cout << "DEBUG: test checking " << proof.PremiseName(conclusion) << endl;
    concchk=proof.TestEvaluate(conclusion,universevec,subjvec,predvec);
    //cout << "DEBUG: " << proof.PremiseName(conclusion) <<"=" << proof.Evaluate(conclusion,universevec,subjvec,predvec) << endl;


    //for every item in the vector start with a vector of 0's
    //fill in one more 1 than the last iteration and check every
    //permutation with the new count of 1's
    for (unsigned i=0; i<=universevec.size(); i++)
    {
        //zero the bools of the vector
        for(unsigned k=0; k<universevec.size(); k++)
        {
            //get<2>(universevec[k])=0;
            myvec[k]=0;
        }

        //add the correct amount of 1's to the zeroized vector
        for(unsigned k=0; k<i; k++)
        {
            //get<2>(universevec[k])=1;
            myvec[k]=1;
        }

        //sort the vector
        sort(myvec.begin(),myvec.end());

        //check each permutation
        do
        {
            //TODO: assign to universe vector instead
            //context.AssignCombination(universevec);
            //cout << "DEBUG: universevec.size()="<<universevec.size()<<endl;
            for(unsigned k=0; k<universevec.size(); k++)
            {
                //cout << "DEBUG:assign "<<"("<<get<0>(universevec[k])<<","<<get<1>(universevec[k])<<","<<get<2>(universevec[k])<<") becomes "<<myvec[k] <<endl;
                get<2>(universevec[k])=myvec[k];
            }

            //cout << "DEBUG myvec=";
            //for (auto it : myvec) cout << it << ",";
            //cout << endl;

            cout << "DEBUG findCounterModel() universe being tested {";
            for (auto it : universevec) cout << "(" <<get<0>(it)<<","<<get<1>(it)<<","<<get<2>(it)<<") ";
            cout << "}" <<endl;


            premchk = true;

            for(int i=0; i < premises; i++)
            {
                //cout << "DEBUG: " << proof.PremiseName(i) <<"=" << proof.Evaluate(i,universevec,subjvec,predvec) << endl;
                cout << "DEBUG: checking " << proof.PremiseName(i) << endl;
                if (!proof.Evaluate(i,universevec,subjvec,predvec)) {
                    premchk = false;
                }
            }

            cout << "DEBUG: checking " << proof.PremiseName(conclusion) << endl;
            concchk=proof.Evaluate(conclusion,universevec,subjvec,predvec);
            //cout << "DEBUG: " << proof.PremiseName(conclusion) <<"=" << proof.Evaluate(conclusion,universevec,subjvec,predvec) << endl;

            //print if we find a counterargument
            if (premchk&&concchk)
            {
                cout << endl;
                cout << "DEBUG: Counter-argument:" << endl;
                //TODO: print universe instead
                //context.DumpContext();
                //cout << endl;
                //cout << "Evaluation:" << endl;

                //for(int i=0; i<premises+1; i++)
                //{
                //cout << proof.PremiseName(i) << "="<<proof.Evaluate(i,universevec,subjvec,predvec)<<endl;
                //}

                counterarg = true;
                countermodel.clear();
                for (auto it : universevec) countermodel.insert(it);
                //cout << endl;
                for(int i=0; i < premises; i++)
                {
                    cout << "DEBUG: " << proof.PremiseName(i) <<"=" << proof.Evaluate(i,universevec,subjvec,predvec) << endl;
                    if (!proof.Evaluate(i,universevec,subjvec,predvec)) {
                        premchk = false;
                    }
                }

                concchk=proof.Evaluate(conclusion,universevec,subjvec,predvec);
                cout << "DEBUG: " << proof.PremiseName(conclusion) <<"=" << proof.Evaluate(conclusion,universevec,subjvec,predvec) << endl;
                cout << "DEBUG: counter-model: {";
                for (auto it : countermodel)
                    cout << " ("<<get<0>(it)<<","<<get<1>(it)<<","<<get<2>(it)<<") ";
                cout <<"}"<< endl<<endl;
            }
        }
        while(next_permutation(myvec.begin(),myvec.end()));
    }
    //throw runtime_error("DEBUG THROW");
    //countermodel.clear();
    //for (auto it : universevec) countermodel.insert(it);
    return counterarg;
}
