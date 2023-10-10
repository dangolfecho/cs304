#include<bits/stdc++.h>
using namespace std;

class OpParser{
	private:
		char start;
		int rightmost_term;
		string input;
		set<char> nonterminals_unique;
		vector<char> nonterminals;
		set<char> terminals_unique;
		vector<char> terminals;
		map<char, bool> leading_done;
		map<char, bool> trailing_done;
		map<char, vector<char>> leading;
		map<char, vector<char>> trailing;
		map<string, char> reversed_productions;
		map<tuple<char, char>, string> table;
		vector<char> stack;
		map<char, vector<string>> grammar;
		void initNonTerms();
		void initTerminals();
		void initReverse();
		void insert(string left, string right);
		void accept();
		bool isNonTerminal(char x);
		bool isTerminal(char x);
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
		void printOutput(int ptr, char left, string right, int status);
		bool parse();
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
	terminals_unique.insert('$');
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
	initTerminals();
}

bool OpParser:: isNonTerminal(char x){
	if(nonterminals_unique.find(x) == nonterminals_unique.end()){
		return false;
	}
	return true;
}

bool OpParser:: isTerminal(char x){
	if(terminals_unique.find(x) == terminals_unique.end()){
		return false;
	}
	return true;
}

void OpParser:: calcLeading(char x){
	char current_nt = x;
	vector<string> current_prods = grammar[x];
	for(int i=0;i<current_prods.size();i++){
		string current_prod = current_prods[i];
		if(isNonTerminal(current_prod[0]) && current_prod.size() == 1){
			if(isLeadingCalc(current_prod[0])){
				vector<char>:: iterator it = leading[current_prod[0]].begin();
				while(it != leading[current_prod[0]].end()){
					leading[current_nt].push_back(*it);
					it++;
				}
				setLeadingDone(current_nt);
			}
		}
		if(isTerminal(current_prod[0])){
			leading[current_nt].push_back(current_prod[0]);
			setLeadingDone(current_nt);
		}
		for(int j=1;j<current_prod.size()-1;j++){
			if(isNonTerminal(current_prod[j-1]) && isTerminal(current_prod[j])){
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
					setLeadingDone(current_nt);
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
		if(current_prod.size() == 1 && isNonTerminal(current_prod[0])){
			if(isTrailingCalc(current_prod[0])){
				vector<char> v = trailing[current_prod[0]];
				for(int j=0;j<v.size();j++){
					trailing[current_nt].push_back(v[j]);
				}
				setTrailingDone(current_nt);
			}

		}
		if(isTerminal(current_prod[current_prod.size()-1])){
			trailing[current_nt].push_back(current_prod[current_prod.size()-1]);
			setTrailingDone(current_nt);
		}
		for(int j=current_prod.size()-2;j>0;j--){
			if(isNonTerminal(current_prod[j+1]) && isTerminal(current_prod[j]) && isNonTerminal(current_prod[j-1])){
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
					if(isTerminal(prod[j]) && isNonTerminal(prod[j+1]) && isTerminal(prod[j+2])){
						t = make_tuple(prod[j], prod[j+2]);
						table[t] = "=";
					}
				}
				if(isTerminal(prod[j]) && isTerminal(prod[j+1])){
					t = make_tuple(prod[j], prod[j+1]);
					table[t] = "=";
				}
				if(isTerminal(prod[j]) && isNonTerminal(prod[j+1])){
					for(int k=0;k<leading[prod[j+1]].size() ;k++){
						t = make_tuple(prod[j], leading[prod[j+1]][k]);
						table[t] = "<.";
					}
				}
				else if(isNonTerminal(prod[j]) && isTerminal(prod[j+1])){
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
		cout << terminals[i] << "  | ";
	}
	cout << "\n";
	for(int i=0;i<terminals.size();i++){
		cout << terminals[i] << " | ";
		for(int j=0;j<terminals.size();j++){
			cout << table[make_tuple(terminals[i], terminals[j])] << " | ";
		}
		cout << "\n";
	}
}

void OpParser:: initReverse(){
	map<char, vector<string>>:: iterator it = grammar.begin();
	while(it != grammar.end()){
		char current_nt = it->first;
		vector<string> v = it->second;
		for(int i=0;i<v.size();i++){
			reversed_productions[v[i]] = current_nt;
		}
		it++;
	}
}

void OpParser:: printOutput(int ptr, char left, string right, int status){
	if(status == 0){
		for(int i=0;i<stack.size();i++){
			cout << stack[i];
		}
		cout << "\t\t";
		char f = input[ptr];
		for(int i=ptr;i<input.size();i++){
			cout << input[i];
		}
		cout << "$\t\t";
		cout << "SHIFT " << left << " " << right << " " << input[ptr] << "\n";
	}
	else{
		for(int i=0;i<stack.size();i++){
			cout << stack[i];
		}
		cout << "\t\t";
		for(int i=ptr;i<input.size();i++){
			cout << input[i];
		}
		cout << "$\t\t";
		cout << "Reduce by " << left << " -> " << right << "\n";
	}
}


bool OpParser:: parse(){
	stack.push_back('$');
	initReverse();
	int ptr = 0;
	tuple<char, char> t;
	cout << "STACK\t\tINPUT\t\tACTION\n";
	while(ptr != input.size() || (stack[0] != '$' || stack[1] != start)){
		int i;
		for(i=stack.size()-1;i>-1;i--){
			if(isTerminal(stack[i])){
				break;
			}
		}
		int val;
		char left;
		string ans;
		if(ptr != input.size()){
			t = make_tuple(stack[i], input[ptr]);
		}
		else{
			t = make_tuple(stack[i], '$');
		}
		if(table[t] == ""){
			cout << "ERROR!\n";
			return false;
		}
		if(table[t] == "<." || table[t] == "="){
			printOutput(ptr, stack[i], table[t], 0);
			stack.push_back(input[ptr]);
			ptr++;
		}
		else{
			val = stack.size()-1;
			while(true){
				if(isTerminal(stack[val])){
					t = make_tuple(stack[val], input[ptr]);
					if(table[t] == "<." || table[t] == "="){
						for(int i=val+1;i<stack.size();i++){
							ans += string(1, stack[i]);
						}
						left =  reversed_productions[ans];
						if(!isNonTerminal(left)){
							ans = "";
							val--;
							continue;
						}
						printOutput(ptr, left, ans, 1);
						int c = ans.size();
						while(c--){
							stack.pop_back();
						}
						stack.push_back(left);
						ans = "";
						break;
					}
					else{
						for(int i=val;i<stack.size();i++){
							ans += string(1, stack[i]);
						}
						left = reversed_productions[ans];
						if(!isNonTerminal(left)){
							ans = "";
							val--;
							continue;
						}
						printOutput(ptr, left, ans, 1);
						int c = ans.size();
						while(c--){
							stack.pop_back();
						}
						stack.push_back(left);
						ans = "";
						if(ptr == input.size() && stack[1] == start){
							cout << "$" << start << "\t\t$\t\tAccept\n";
							return true;
						}
					}
				}
				val--;
			}
			if(ptr != input.size() && left == start){
				if(isNonTerminal(reversed_productions[string(1, left)])){
					printOutput(ptr, reversed_productions[string(1, left)], "S", 1);
					stack.pop_back();
					stack.push_back(reversed_productions[string(1, left)]);
				}
			}
		}
	}
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
	while(!isLeadingDone()){
		for(int i=0;i<nonterminals.size();i++){
			if(!isLeadingCalc(nonterminals[i])){
				calcLeading(nonterminals[i]);
			}
		}
	}
	while(!isTrailingDone()){
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
	while(true){
		cout << "Enter the string to be checked\n";
		cin >> input;
		bool f = parse();
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
