#include <iostream>
#include <fstream>
#include <regex> //should have been using this from the start

using namespace std;
int findEndOfQuant(string);
/**read a formula from the user into a string
*	@param	the input stream to read from (probably not necessary)
*	@param	the string to read into
*	@return currently returns 0 always but may return something more useful in the future
*/
int readFormula(istream& instrm,string& str);
/**recursively parse a formula and emit the correct sequence of characters to a file
*	@param fout the file stream to emit to
*	@param the string to parse
*	@param the start position in the string to parse from (possibly unnecessary)
*	@param the ending position in the string to parse from (possibly unnecessary)
*	@return currently returns 1 always but may return something more useful in the future
*/
int parseFormula(ofstream& fout,const string& str, int start, int stop);
/**ask the user if the previously entered formula was the last
*	@return true if the user choose so returns false if not
*/
bool isLastFormula();
/**given a string and the index of an open paren return the index of it's matching paren
*	@param str the string being analyzed
*	@param start the opening paren to be matched
*	@return returns the index of the required closing paren or -1 if it cannot find a closing paren
*/
int matchEndParens(const string& str,int start);
/**emit the heredoc preamble code to a file
*	@param fout the ofstream to output to
*/
void expectPreamble(ofstream& fout);
/**remove space characters from the input string
*	@param str the string having whitespace removed
*/
void removeWhiteSpace(string& str);

/**check if the given string is a single propostional variable
*	@param str the string to be analyzed
*	@return returns 1 if the input string is a variable and -1 if not
*/
int isUniversal(const string& str);
int isExistential(const string& str);
int isQuantified(const string& str);
int isVariable(const string& str);
/**check if the given string is a conditional
*	@param str the string to be analyzed
*	@return returns 1 if the input string is a conditional and -1 if not
*/
int isConditional(const string&);
/**check if the given string is an and
*	@param str the string to be analyzed
*	@return returns 1 if the input string is an and returns -1 if not
*/
int isAnd(const string&);
/**check if the given string is an or
*	@param str the string to be analyzed
*	@return returns 1 if the input string is an or returns -1 if not
*/
int isOr(const string&);
/**check if the given string is a not
*	@param str the string to be analyzed
*	@return returns 1 if the input string is a not and -1 if not
*/
int isNot(const string&);

/**emit the correct character sequence for a two character operator
*	@param fout the stream to output to
*	@param str the string being analyzed
*	@param start the starting point in the string to work on
*	@param oploc the location of the first character of the operator in the expression
*	@param op the operator that will be emitted
*/
void twoplaceemit(ofstream& fout,const string& str,int start,int oploc,const string& op);
/**emit the correct character sequence for a one character operator
*	@param fout the stream to output to
*	@param str the string being analyzed
*	@param start the starting point in the string to work on
*	@param oploc the location of the operator in the expression
*	@param op the operator that will be emitted
*/
void oneplaceemit(ofstream& fout,const string& str,int start,int oploc,const string& op);

int main()
{
    bool last = false;
    ofstream outfile;
    string input = "";
    int i = 0;

    //open the output file
    outfile.open("output_expect.txt");

    expectPreamble(outfile);

    //read parse and emit every premise
    //ask the user if they want to go to the conclusion each
    //iteration
    while (!last)
    {
        cout << "Please Enter a formula:" << endl;
        readFormula(cin,input);
        removeWhiteSpace(input);
        parseFormula(outfile,input,0,input.size());
        last = isLastFormula();

        if (!last)
        {
            cout << "FILE:send \"c\\r\"; #more formulas" << endl;
            outfile << "send \"c\\r\"; #more formulas" << endl;
        }
        else
        {
            cout << "FILE: send \"q\\r\"; #last formula" << endl;
            outfile << "send \"q\\r\"; #last formula" << endl;
        }

        i++;
    }

    //generic ending code for the file
    cout << "FILE: interact; #hand input to user " << endl;
    outfile << "interact; #hand input to user " << endl;

    //we are done close the file and exit
    outfile.close();
    return 0;
}

//returns the position of amatching end paren given a string and
//the position of a starting paren
int matchEndParens(const string& str,int start)
{
    int nesting = 0;
    int ending = -1;
    //cout << "\tDEBUG:(matchparen): ";
    for (unsigned int i = start; i < str.size(); i++)
    {
        //cout << str[i];
        if ( str[i]=='(')
        {
            nesting++;
        }
        else if ( str[i]==')')
        {
            nesting--;
        }
        if (nesting==0)
        {
            ending = i;
            break;
        }
    }
    //cout << endl;
    return ending;
}

//get a formula from the user
int readFormula(istream& instrm,string& str)
{
    getline(instrm,str,'\n');
    return 0;
}

//takes a formula from the user outputs correct code to the file for the outermost
//expression type and then recurses on any subexpressions
int parseFormula(ofstream& fout,const string& str, int start, int stop)
{
    cout << endl;
    int oploc = -1;

    if ((oploc = isExistential(str))!=-1)
    {
        int endparen = 0;
        //cout << "DEBUG: str="<<str<<endl;
        endparen = matchEndParens(str,0);
        //cout << "DEBUG: endparen="<<endparen<<endl;
        //cout << "DEBUG: var="<<str.substr(5,endparen-5)<<"#"<<endl;
        //cout << "DEBUG: str[endparen+1]="<<str.substr(endparen+1,str.size())<<endl;
        cout << "FILE: send \"1\\r\"; #unary" << endl;
        fout << "send \"1\\r\"; #unary" << endl;
        cout << "FILE: send \"e\\r\"; #existential" << endl;
        fout << "send \"e\\r\"; #existential" << endl;
        cout << "FILE: send \"" <<str.substr(5,endparen-5)<<"\\r\"; #dependent variable"<< endl;
        fout << "send \"" <<str.substr(5,endparen-5)<<"\\r\"; #dependent variable"<< endl;
        string sub = str.substr(endparen+1,str.size());
        //cout << "\tDEBUG:(matched exis) sub="<<sub<<endl;
        parseFormula(fout,sub,0,0);
    }
    else if ((oploc = isUniversal(str))!=-1)
    {
        int endparen = 0;
        //cout << "DEBUG: str="<<str<<endl;
        endparen = matchEndParens(str,0);
        //cout << "DEBUG: endparen="<<endparen<<endl;
        //cout << "DEBUG: var="<<str.substr(4,endparen-4)<<"#"<<endl;
        //cout << "DEBUG: str[endparen+1]="<<str.substr(endparen+1,str.size())<<endl;
        cout << "FILE: send \"1\\r\"; #unary" << endl;
        fout << "send \"1\\r\"; #unary" << endl;
        cout << "FILE: send \"u\\r\"; #universal" << endl;
        fout << "send \"u\\r\"; #universal" << endl;
        cout << "FILE: send \"" <<str.substr(4,endparen-4)<<"\\r\"; #dependent variable"<< endl;
        fout << "send \"" <<str.substr(4,endparen-4)<<"\\r\"; #dependent variable"<< endl;
        string sub = str.substr(endparen+1,str.size());
        //cout << "\tDEBUG:(matched univ) sub="<<sub<<endl;
        parseFormula(fout,sub,0,0);
    }
    else if (isVariable(str)!=-1)
    {
        unsigned startparen=0;
        //unsigned endparen=0;
        cout << "FILE: send \"1\\r\"; #unary" << endl;
        fout << "send \"1\\r\"; #unary" << endl;
        cout << "FILE: send \"v\\r\"; #variable" << endl;
        fout << "send \"v\\r\"; #variable" << endl;
        //for (startparen = 0; startparen<str.size()&&str[startparen]!='(';startparen++);
        for (startparen = 0; startparen<str.size()-1&&str[startparen]!='('; startparen++);
        //endparen = matchEndParens(str,0);
        cout << "FILE: send\"" << str.substr(0,startparen)<<"\\r\"; #relation name" << endl;
        fout << "send \"" << str.substr(0,startparen) <<"\\r\"; #relation name"<< endl;
        cout << "FILE: send \""<< str.substr(startparen+1,str.size()-startparen-2) <<"\\r\"; #tuple"<< endl;
        fout << "send \""<<str.substr(startparen+1,str.size()-startparen-2) <<"\\r\"; #tuple"<< endl;
        return 1;
    }
    else if ((oploc = isNot(str))!=-1)
    {
        //cout << "\tDEBUG: MATCHED NOT"<<endl;
        string sub = str.substr(1,str.size()-1);
        //cout << "DEBUG: str="<<str<<endl;
        //cout << "DEBUG: sub="<<sub<<endl;
        cout << "FILE: send \"1\\r\"; #unary" << endl;
        fout << "send \"1\\r\"; #unary" << endl;
        cout << "FILE: send \"n\\r\"; #not" << endl;
        fout << "send \"n\\r\"; #not" << endl;
        parseFormula(fout,sub,0,0);
        return 1;
    }
    else if ((oploc = isConditional(str))!=-1)
    {
        //cout << "\tDEBUG: MATCHED COND"<<endl;
        twoplaceemit(fout,str,start,oploc,"c");
        return 1;
    }
    else if ((oploc = isOr(str))!=-1)
    {
        //cout << "\tDEBUG: MATCHED OR"<<endl;
        oneplaceemit(fout,str,start,oploc,"o");
        return 1;
    }
    else if ((oploc = isAnd(str))!=-1)
    {
        //cout << "\tDEBUG: MATCHED AND"<<endl;
        oneplaceemit(fout,str,start,oploc,"a");
        return 1;
    }
    return 1;
}

//ask the user if this is the last formula returns true or false
bool isLastFormula()
{
    bool ret = false;
    string str = "";
    do
    {
        cout << "Was that the last formulas? (y/n)" << endl;
        getline(cin,str,'\n');
    } while(str[0]!='n'&&str[0]!='Y'&&str[0]!='N'&&str[0]!='y');

    if (str=="y"||str=="Y")ret=true;
    else ret = false;

    return ret;
}

//every heredoc starts the same way
void expectPreamble(ofstream& fout)
{
    cout << "FILE: #!/bin/expect" << endl;
    fout << "#!/bin/expect" << endl;
    cout << "FILE: spawn ../main" << endl;
    fout << "spawn ../main" << endl;
    fout << "send \"c\\r\"; #this program is always calc mode" << endl;
}
int isQuantified(const string& str)
{
    //cout << "\tDEBUG: isQuantified:" << str <<endl;
    if (isExistential(str)==1) return 1;
    else if (isUniversal(str)==1) return 1;
    else return -1;
}
int isExistential(const string& str)
{
    //if (regex_match(str,regex("(Some.*)")))
    //if (regex_search(str,regex("^\\(Some(.*)\\)")))
    if (str.substr(0,5)=="(Some")
    {
        //cout << "\tDEBUG: MATCHED EXISTENTIAL" << endl;
        return 1;
    }
    return -1;
}
int isUniversal(const string& str)
{
    //if (regex_match(str,regex("(Some.*)")))
    //if (regex_search(str,regex("^\\(All(.*)\\)")))
    if (str.substr(0,4)=="(All")
    {
        //cout << "\tDEBUG: MATCHED UNIVERSAL" <<endl;
        return 1;
    }
    return -1;
}

//identify if the formula is a variable return 1 if true -1 if false
int isVariable(const string& str)
{
    if (isalpha(str[0]) && isupper(str[0])) return 1;
    else return -1;
}
//identify if the formula is a conditional return the position of
//the '-' part of the conditional operator if true return -1 if false
int isConditional(const string& str)
{
    int retval = -1;
    int nesting = 0;
    int start = 0;
    for (unsigned int i = start; i < str.size()&&str[0]=='('; i++)
    {
        if(i==str.size())
        {
            i=-1;
            break;
        }
        if ( str[i]=='(') nesting++;
        if ( str[i]==')'&&i!=str.size()) nesting--;
        if (i<str.size() && str[i]== '-'&& nesting==1) retval = i;
    }
    return retval;
}
//identify if the formula is an and return the position of the & operator if true
//return -1 if false
int isAnd(const string& str)
{
    int retval = -1;
    int nesting = 0;
    int start = 0;
    for (unsigned int i = start; i < str.size()&&str[0]=='('; i++)
    {
        if(i==str.size())
        {
            i=-1;
            break;
        }
        if ( str[i]=='(') nesting++;
        if ( str[i]==')'&&i!=str.size()) nesting--;
        if (i<str.size() && str[i]== '&'&&nesting==1) retval = i;
    }
    return retval;
}
//identify if the formula is an or return the position of v operator if true
//return -1 if false
int isOr(const string& str)
{
    int retval = -1;
    int nesting = 0;
    int start = 0;
    for (unsigned int i = start; i < str.size()&&str[0]=='('; i++)
    {
        if(i==str.size())
        {
            i=-1;
            break;
        }
        if ( str[i]=='(') nesting++;
        if ( str[i]==')'&&i!=str.size()) nesting--;
        if (i<str.size() && str[i]== 'v'&& nesting==1) retval = i;
    }
    return retval;

}
//identify if the formula is a not expression return 1 if true return -1 if false
int isNot(const string& str)
{
    if (str[0] == '~') return 1;
    else return -1;
}
//remove any spaces from a given string
void removeWhiteSpace(string& str)
{
    for(unsigned int i = 0; i < str.size(); i++)
    {
        if(str[i]==' ')str.erase(i,1);
    }
}
//emit and recurse to parse any children formulas this version is for two character operators
void twoplaceemit(ofstream& fout,const string& str,int start,int oploc, const string& op)
{
    //cout <<"\tDEBUG: twoplaceemit:"<<endl;
    //cout <<"\tDEBUG:(twoplaceemit) str="<<str<<endl;
    cout << "FILE: send \"2\\r\"; #binary" << endl;
    fout << "send \"2\\r\"; #binary" << endl;
    cout << "FILE: send\""<<op<<"\\r\"; #binary connective" << endl;
    fout << "send \"" << op <<"\\r\"; #binary connective" << endl;

    string sub;
    int lstart = 1;
    //if there is an open paren after lend or a letter then lend marks a quantifier
    //and more matching must be done
    int lend = lstart;
    //cout <<"\tDEBUG: lstart="<<lstart<<endl;
    //cout <<"\tDEBUG: lend="<<lend<<endl;
    //cout <<"\tDEBUG: oploc="<<oploc<<endl;
    if (isQuantified(str.substr(lstart,oploc-1))==1||(str[lstart]=='~'&&isQuantified(str.substr(lstart+1,oploc-1))==1))
    {
        lend =  findEndOfQuant(str.substr(lstart,oploc-1));
        sub = str.substr(lstart,lend);
        //cout <<"\tDEBUG: lend="<<lend<<endl;
    }
    else
    {
        for (; str[lend]!='('; lend++);
        lend = matchEndParens(str,lend);
        sub = str.substr(lstart,lend);
    }
    //cout <<"\tDEBUG:(left) str.substr(lstart,lend+5)"<<str.substr(lstart,lend+5)<<endl;
    //cout <<"\tDEBUG:(left) sub="<<sub<<endl;
    parseFormula(fout,sub,0,0);

    int rstart = oploc+2;
    //if there is an open paren after rend or a letter then rend marks a quantifier
    //and more matching must be done
    int rend = rstart;
    if (isQuantified(str.substr(rstart,str.size()))==1||(str[rstart]=='~'&&isQuantified(str.substr(rstart+1,str.size()))==1))
    {
        rend = findEndOfQuant(str.substr(rstart,str.size()-rstart-1));
        sub = str.substr(rstart,rend);
    }
    else
    {
        for (; str[rend]!='('; rend++);
        rend = matchEndParens(str,rend);
        sub = str.substr(rstart,rend-rstart+1);
    }
    //cout <<"\tDEBUG:(right) sub="<<sub<<endl;
    parseFormula(fout,sub,0,0);

}
//emit and recurse to parse any children formulas this version is for one character operators
void oneplaceemit(ofstream& fout,const string& str,int start,int oploc, const string& op)
{
    //cout <<"\tDEBUG: oneplaceemit:"<<endl;
    //cout <<"\tDEBUG:(oneplacemit) str="<<str<<endl;
    cout << "FILE: send \"2\\r\"; #binary" << endl;
    fout << "send \"2\\r\"; #binary" << endl;
    cout << "FILE: send \"" << op <<"\\r\"; #binary connective"<< endl;
    fout << "send \"" << op <<"\\r\"; #binary connective"<< endl;

    string sub;
    int lstart = 1;
    //if there is an open paren after lend or a letter then lend marks a quantifier
    //and more matching must be done
    int lend = lstart;
    if (isQuantified(str.substr(lstart,oploc-1))==1||(str[lstart]=='~'&&isQuantified(str.substr(lstart+1,oploc-1))==1))
    {
        lend =  findEndOfQuant(str.substr(lstart,oploc-1));
        sub = str.substr(lstart,lend);
    }
    else
    {
        for (; str[lend]!='('; lend++);
        lend = matchEndParens(str,lend);
        sub = str.substr(lstart,lend);
    }
    //cout <<"\tDEBUG:(left) sub="<<sub<<endl;
    parseFormula(fout,sub,0,0);

    int rstart = oploc+1;
    //if there is an open paren after rend or a letter then rend marks a quantifier
    //and more matching must be done
    int rend = rstart;
    if (isQuantified(str.substr(rstart,str.size()))==1||(str[rstart]=='~'&&isQuantified(str.substr(rstart+1,str.size()))==1))
    {
        rend = findEndOfQuant(str.substr(rstart,str.size()-rstart-1));
        sub = str.substr(rstart,rend);
    }
    else
    {
        for (; str[rend]!='('; rend++);
        rend = matchEndParens(str,rend);
        sub = str.substr(rstart,rend-rstart+1);
    }
    //cout <<"\tDEBUG:(right) sub="<<sub<<endl;
    parseFormula(fout,sub,0,0);
}
int findEndOfQuant(string str)
{
    //cout << "\tDEBUG:int findEndOfQuant(string str)"<<endl;
    //cout << "\tDEBUG: str="<<str<<endl;
    //cout <<endl<< "\t###################################################################"<<endl;
    //cout << "\tDEBUG: findendofquant"<<endl;
    //cout << "\tDEBUG:(findend) str="<<str<<endl;
    int startparen=0;
    int endparen=0;
    for (; str[startparen]!='('; startparen++); //find the start paren

    //cout << "\tDEBUG:(findend) pre: startparen="<<startparen<<endl;
    //cout << "\tDEBUG:(findend) pre: endparen="<<endparen<<endl;
    //cout << "\tDEBUG:(findend) pre: str[matchEndParens(str,startparen)+1]"<<str[matchEndParens(str,startparen)+1]<<endl;
    //keep trotting along the quantified expression matching endparens
    //until neither ")~", ")(" or ")X" occurs
    for (int temp = startparen;
            str[matchEndParens(str,temp)+1]=='(' ||
            str[matchEndParens(str,temp)+1]=='~' ||
            isalpha(str[matchEndParens(str,temp)+1]);
            endparen=temp = matchEndParens(str,temp)+1)
    {
        //cout << "\tDEBUG: start loopbody"<<endl;
        //endparen=temp;
        //cout <<"\tDEBUG:(findend) endparen="<<endparen<<endl;
        //cout <<"\tDEBUG:(findend) temp="<<temp<<endl;
        //cout <<"\tDEBUG:(findend) str[temp]="<<str[temp]<<endl;
        //cout <<"\tDEBUG:(findend) str[endparen]="<<str[endparen]<<endl<<endl;
        //cout << "\tDEBUG:(findend) pre: str[matchEndParens(str,temp)+1]"<<str[matchEndParens(str,temp)+1]<<endl;
    }
    //cout <<"\tDEBUG:(findend) post: endparen="<<endparen<<endl;
    //cout <<"\tDEBUG:(findend) post: endparen="<<endparen<<endl;
    //cout <<"\tDEBUG:(findend) post: str[endparen]="<<str[endparen]<<endl;
    //cout <<"\tDEBUG:(findend) post: str[endparen+1]="<<str[endparen+1]<<endl;
    //cout <<"\tDEBUG:(findend) post: str[endparen+2]="<<str[endparen+2]<<endl;
    //cout <<"\tDEBUG:(findend) post: str[endparen+3]="<<str[endparen+3]<<endl;
    //cout <<"\tDEBUG:(findend) post: str[endparen+4]="<<str[endparen+4]<<endl;
    endparen = matchEndParens(str,endparen)+1;
    //endparen = matchEndParens(str,endparen+1)+1;
    //cout <<"\tDEBUG:(findend) match: endparen="<<endparen<<endl;
    //cout <<"\tDEBUG:(findend) str[endparen]="<<str[endparen]<<endl;
    //cout << "\t###################################################################"<<endl;
    return endparen;
}
