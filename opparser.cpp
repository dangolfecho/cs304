#include<bits/stdc++.h>
using namespace std;

class OpParser{
	private:
		char start;
		set<char> nonterminals_unique;
		vector<char> nonterminals;
		set<char> terminals_unique;
		vector<char> terminals;
		map<char, bool> leading_done;
		map<char, bool> trailing_done;
		map<char, vector<char>> leading;
		map<char, vector<char>> trailing;
		map<tuple<char, char>, string> table;
		vector<char> stack;
		map<char, vector<string>> grammar;
		void initNonTerms();
		void initTerminals();
		void insert(string left, string right);
		void accept();
		bool isNonTerminal(char x);
		void setLeadingDone(char x);
		void setTrailingDone(char x);
		bool isLeadingDone();
		bool isTrailingDone();
		bool isLeadingCalc(char x);
		bool isTrailingCalc(char x);
		void calcLeading(char x);
		void calcTrailing(char x);
		void printLeading();
		void printTrailing();
		void simplify();
		void calcTable();
		void printTable();
		bool parse(string x);
		bool isOpGrammar();
	public:
		void startFunc();
};

void OpParser:: setLeadingDone(char x){
	leading_done[x] = true;
}

void OpParser:: setTrailingDone(char x){
	trailing_done[x] = true;
}

bool OpParser:: isLeadingDone(){
	for(int i=0;i<nonterminals.size();i++){
		if(!leading_done[nonterminals[i]]){
			return false;
		}
	}
	cout << "HERE\n";
	return true;
}

bool OpParser:: isTrailingDone(){
	for(int i=0;i<nonterminals.size();i++){
		if(!trailing_done[nonterminals[i]]){
			return false;
		}
	}
	return true;
}

bool OpParser:: isLeadingCalc(char x){
	return leading_done[x];
}

bool OpParser:: isTrailingCalc(char x){
	return trailing_done[x];
}

void OpParser:: initNonTerms(){
	set<char>:: iterator it = nonterminals_unique.begin();
	while(it != nonterminals_unique.end()){
		nonterminals.push_back(*it);
		leading_done[*it] = false;
		trailing_done[*it] = false;
		it++;
	}
}

void OpParser:: initTerminals(){
	map<char, vector<string>>::iterator it = grammar.begin();
	while(it != grammar.end()){
		vector<string> prods =  it->second;
		for(int i=0;i<prods.size();i++){
			for(int j=0;j<prods[i].size();j++){
				if(!isNonTerminal(prods[i][j])){
					terminals_unique.insert(prods[i][j]);
				}
			}
		}
		it++;
	}
	set<char>:: iterator it1 = terminals_unique.begin();
	while(it1 != terminals_unique.end()){
		terminals.push_back(*(it1));
		it1++;
	}
	terminals.push_back('$');
}

void OpParser:: simplify(){
	set<char> simple;
	set<char>:: iterator s;
	map<char, vector<char>>:: iterator it = leading.begin();
	while(it != leading.end()){
		simple.clear();
		vector<char> x = it->second;
		for(int i=0;i<x.size();i++){
			simple.insert(x[i]);
		}
		leading[it->first].clear();
		for(s = simple.begin(); s != simple.end(); s++){
			leading[it->first].push_back(*s);
		}
		it++;
	}
	it = trailing.begin();
	while(it != trailing.end()){
		simple.clear();
		vector<char> x = it->second;
		for(int i=0;i<x.size();i++){
			simple.insert(x[i]);
		}
		trailing[it->first].clear();
		for(s = simple.begin(); s != simple.end(); s++){
			trailing[it->first].push_back(*s);
		}
		it++;
	}
}

void OpParser:: insert(string left, string right){
	nonterminals_unique.insert(left[0]);
	grammar[left[0]].push_back(right);
}

void OpParser:: accept(){
	string left;
	string right;
	cout << "Enter # to end\n";
	while(true){
		cout << "Enter the LHS of the production\n";
		cin >> left;
		if(left == "#"){
			break;
		}
		if(left.size() > 1){
			cout << "Enter a valid production\n";
			continue;
		}
		cout << "Enter the RHS of the production\n";
		cin >> right;
		insert(left, right);
	}
	string s;
	while(true){
		cout << "Enter the start symbol\n";
		cin >> s;
		if(s.size() == 1){
			start = s[0];
			break;
		}
		cout << "Enter a valid start symbol\n";
	}
	initNonTerms();
}

bool OpParser:: isNonTerminal(char x){
	if(nonterminals_unique.find(x) == nonterminals_unique.end()){
		return false;
	}
	return true;
}

void OpParser:: calcLeading(char x){
	char current_nt = x;
	vector<string> current_prods = grammar[x];
	cout << "hi\n";
	for(int i=0;i<current_prods.size();i++){
		string current_prod = current_prods[i];
		cout << current_prod << "\n";
		if(!isNonTerminal(current_prod[0])){
			leading[current_nt].push_back(current_prod[0]);
			setLeadingDone(current_nt);
		}
		for(int j=1;j<current_prod.size()-1;j++){
			if(isNonTerminal(current_prod[j-1]) && isNonTerminal(current_prod[j+1])){
				leading[current_nt].push_back(current_prod[j]);
				if(isLeadingCalc(current_prod[j-1])){
					vector<char>:: iterator it = leading[current_prod[j-1]].begin();
					while(it != leading[current_prod[j-1]].end()){
						leading[current_nt].push_back(*it);
						it++;
					}
					setLeadingDone(current_nt);
				}
				else{
					if(current_prod[j-1] != current_nt){
						calcLeading(current_prod[j-1]);
					}
				}
			}
		}
	}
}

void OpParser:: calcTrailing(char x){
	char current_nt = x;
	vector<string> current_prods = grammar[x];
	for(int i=0;i<current_prods.size();i++){
		string current_prod = current_prods[i];
		if(!isNonTerminal(current_prod[current_prod.size()-1])){
			trailing[current_nt].push_back(current_prod[current_prod.size()-1]);
			setTrailingDone(current_nt);
		}
		for(int j=1;j<current_prod.size()-1;j++){
			if(isNonTerminal(current_prod[j-1]) && isNonTerminal(current_prod[j+1])){
				trailing[current_nt].push_back(current_prod[j]);
				if(isTrailingCalc(current_prod[j+1])){
					vector<char>:: iterator it = trailing[current_prod[j+1]].begin();
					while(it != trailing[current_prod[j+1]].end()){
						trailing[current_nt].push_back(*it);
						it++;
					}
					setTrailingDone(current_nt);
				}
				else{
					if(current_prod[j+1] != current_nt){
						calcTrailing(current_prod[j+1]);
					}
				}
			}
		}
	}
}

void OpParser:: calcTable(){
	initTerminals();
	simplify();
	tuple<char, char> t;
	for(int i=0;i<leading[start].size();i++){
		t = make_tuple('$', leading[start][i]);
		table[t] = "<.";
	}
	for(int i=0;i<trailing[start].size();i++){
		t = make_tuple(trailing[start][i], '$');
		table[t] = ".>";
	}
	map<char, vector<string>>::iterator it = grammar.begin();
	while(it != grammar.end()){
		vector<string> prods = it->second;
		for(int i=0;i<prods.size();i++){
			string prod = prods[i];
			for(int j=0;j<(prod.size()-1);j++){
				if(j+2 < prod.size()){
					if(!isNonTerminal(prod[j]) && isNonTerminal(prod[j+1]) && !isNonTerminal(prod[j+2])){
						t = make_tuple(prod[j], prod[j+2]);
						table[t] = "=";
					}
				}
				else if(!isNonTerminal(prod[j]) && !isNonTerminal(prod[j+1])){
					t = make_tuple(prod[j], prod[j+1]);
					table[t] = "=";
				}
				else if(!isNonTerminal(prod[j]) && isNonTerminal(prod[j+1])){
					for(int k=0;j<leading[prod[j+1]].size() ;k++){
						t = make_tuple(prod[j], leading[prod[j+1]][k]);
						table[t] = "<.";
					}
				}
				else if(isNonTerminal(prod[j]) && !isNonTerminal(prod[j+1])){
					for(int k=0;k<trailing[prod[j]].size(); k++){
						t = make_tuple(trailing[prod[j]][k], prod[j+1]);
						table[t] = ".>";
					}
				}
			}
		}
		it++;
	}
}

void OpParser:: printLeading(){
	cout << "Nonterminal\t\tLeading()\n";
	for(int i=0;i<nonterminals.size();i++){
		cout << nonterminals[i] << "\t\t";
		for(int j=0;j<leading[nonterminals[i]].size();j++){
			cout << leading[nonterminals[i]][j] << " ";
		}
		cout << "\n";
	}
}

void OpParser:: printTrailing(){
	cout << "Nonterminal\t\tTrailing()\n";
	for(int i=0;i<nonterminals.size();i++){
		cout << nonterminals[i] << "\t\t";
		for(int j=0;j<trailing[nonterminals[i]].size();j++){
			cout << trailing[nonterminals[i]][j] << " ";
		}
		cout << "\n";
	}
}

void OpParser:: printTable(){
	cout << " ";
	for(int i=0;i<terminals.size();i++){
		cout << terminals[i] << "  ";
	}
	cout << "\n";
	for(int i=0;i<terminals.size();i++){
		cout << terminals[i] << " ";
		for(int j=0;j<terminals.size();j++){
			cout << table[make_tuple(terminals[i], terminals[j])] << " ";
		}
		cout << "\n";
	}
}

bool OpParser:: parse(string input){
	cout << "HI\n";
	return true;	
}

bool OpParser:: isOpGrammar(){
	map<char, vector<string>>:: iterator it = grammar.begin();
	while(it != grammar.end()){
		vector<string> current_prods = it->second;
		for(int i=0;i<current_prods.size();i++){
			string current_prod = current_prods[i];
			if(current_prod == "epsilon"){
				return false;
			}
			for(int j=0;j<(current_prod.size()-1);j++){
				if(isNonTerminal(current_prod[j]) && isNonTerminal(current_prod[j+1])){
					cout << current_prod[j] << " " << current_prod[j+1] << "\n";
					return false;
				}
			}
		}
		it++;
	}
	return true;
}

void OpParser:: startFunc(){
	accept();
	if(!isOpGrammar()){
		cout << "The given grammar is not an operator grammar!\n";
		return;
	}
	cout << "HERE\n";
	while(isLeadingDone()){
		cout << "inHERE\n";
		for(int i=0;i<nonterminals.size();i++){
			if(!isLeadingCalc(nonterminals[i])){
				calcLeading(nonterminals[i]);
			}
		}
	}
	while(isTrailingDone()){
		for(int i=0;i<nonterminals.size();i++){
			if(!isTrailingCalc(nonterminals[i])){
				calcTrailing(nonterminals[i]);
			}
		}
	}
	calcTable();
	printLeading();
	printTrailing();
	printTable();
	string input;
	while(true){
		cout << "Enter the string to be checked\n";
		cin >> input;
		bool f = parse(input);
		if(f){
			cout << "The string has been successfully parsed!\n";
		}
		else{
			cout << "The string does not conform to the given grammar\n";
		}
	}
}

int main(){
	OpParser o;
	o.startFunc();
	return 0;
}
