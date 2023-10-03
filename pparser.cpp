//find first
//find follow
//create predictive parser table
//simulate accepting a string
#include<bits/stdc++.h>
using namespace std;

class PredParser{
	private:
		char start;
		char left;
		string right;
		string input;
		bool flag;
		int input_ptr;
		bool input_flag;
		bool ep_flag;
		set<char> set_nonterminals;
		set<char> set_terminals;
		vector<char> nonterminals;
		vector<char> terminals;
		vector<char> parse_stack;
		map<char, bool> first_calculation;
		map<char, bool> follow_calculation;
		map<char, vector<string>> grammar;
		map<char, vector<pair<char,string>>> follow_instances;
		map<char, vector<string>> first;
		map<char, vector<string>> follow;
		map<tuple<char, char>, vector<string>> table;
		bool isNonTerminal(char x);
		int getIndex(char x);
		void setIndex();
		void calcFirst(int index);
		void calcFollow(int index);
		void simplify();
		void calcTable();
		void printFirst();
		void printFollow();
		void printTable();
		void printParseOutput(string prod);
		void setFirstDone(char x);
		void setFollowDone(char x);
		bool firstContainsEmpty(char x);
		bool isFirstDone();
		bool isFollowDone();
		bool isFirstCalculated(char x);
		bool isFollowCalculated(char x);
		void insert(char left, string right);
		void findTerminals();
		void createFollowPrereq();
		bool parse();
		void get();
	public:
		void startFunc();
};

void PredParser:: findTerminals(){
	map<char, vector<string>>:: iterator it = grammar.begin();
	while(it != grammar.end()){
		vector<string> b = it->second;
		for(int i=0;i<b.size();i++){
			string cur = b[i];
			if(cur == "epsilon"){
				continue;
			}
			for(int j=0;j<cur.size();j++){
				if(!isNonTerminal(cur[j])){
					set_terminals.insert(cur[j]);
				}
			}
		}
		it++;
	}
	for(auto x:set_terminals){
		terminals.push_back(x);
	}
	terminals.push_back('$');
}

void PredParser:: setFirstDone(char x){
	first_calculation[x] = true;
}

void PredParser:: setFollowDone(char x){
	follow_calculation[x] = true;
}

bool PredParser:: firstContainsEmpty(char x){
	vector<string>:: iterator it = first[x].begin();
	bool t_flag = false;
	while(it != first[x].end()){
		if((*it) == "epsilon"){
			t_flag = true;
			break;
		}
		it++;
	}
	return t_flag;
}

bool PredParser:: isFirstDone(){
	bool t_flag = true;
	map<char, bool>:: iterator it = first_calculation.begin();
	while(it != first_calculation.end()){
		bool right = it->second;
		if(!right){
			t_flag = false;
			break;
		}
		it++;
	}
	return t_flag;
}

bool PredParser:: isFollowDone(){
	bool t_flag = true;
	map<char, bool>:: iterator it = follow_calculation.begin();
	while(it != follow_calculation.end()){
		bool right = it->second;
		if(!right){
			t_flag = false;
			break;
		}
		it++;
	}
	return t_flag;
}

bool PredParser:: isFirstCalculated(char x){
	bool ans = first_calculation[x];
	return ans;	
}

bool PredParser:: isFollowCalculated(char x){
	bool ans = follow_calculation[x];
	return ans;
}

int PredParser:: getIndex(char x){
	int i =0;
	for(i=0;i<nonterminals.size();i++){
		if(x == nonterminals[i]){
			break;
		}
	}
	return i;
}

void PredParser:: setIndex(){
	nonterminals.push_back(start);
	set<char>::iterator it = set_nonterminals.begin();
	while(it!=set_nonterminals.end()){
		if(*it != start){
			nonterminals.push_back(*it);
		}
		it++;
	}
}

bool PredParser:: isNonTerminal(char x){
	if(set_nonterminals.find(x) == set_nonterminals.end()){
		return false;
	}
	return true;
}

void PredParser:: insert(char left, string right){
	grammar[left].push_back(right);
}

void PredParser:: calcFirst(int index){
	char current_nt = nonterminals[index];
	vector<string> current_char_prods = grammar[current_nt];
	string current_prod;
	for(int i=0; i<current_char_prods.size();i++){
		current_prod = current_char_prods[i];
		if(current_prod == "epsilon"){
			first[current_nt].push_back("epsilon");
			continue;
		}
		for(int j=0;j<current_prod.size();j++){
			ep_flag = false;
			char current_char = current_prod[j];
			if(isNonTerminal(current_char)){
				if(first_calculation[current_char]){
					for(int k=0;k<first[current_char].size();k++){
						if(first[current_char][k] == "epsilon"){
							ep_flag = true;
						}
						first[current_nt].push_back(first[current_char][k]);
					}
					if(!ep_flag){
						break;
					}
				}
				else{
					int recur_index = getIndex(current_char);
					calcFirst(recur_index);
					j--;
				}
			}
			else{
				first[current_nt].push_back(string(1, current_char));
				break;
			}
		}
	}
	setFirstDone(current_nt);
}

void PredParser:: calcFollow(int index){
	char current_nt = nonterminals[index];
	vector<pair<char,string>> current_follow_instances = follow_instances[current_nt];
	vector<pair<char,string>>::iterator it = current_follow_instances.begin();
	bool t_flag = false;
	while(it != current_follow_instances.end()){
		pair<char, string> p = *it;
		char left_cur = p.first;
		string prod = p.second;
		for(int i=0;i<prod.size();i++){
			if(prod[i] == current_nt){
				cout << left_cur << " " << prod << " " << current_nt << "\n";
				if(i == (prod.size()-1)){
					if(isFollowCalculated(left_cur)){
						vector<string> x = follow[left_cur];
						for(int j=0;j<x.size();j++){
							follow[current_nt].push_back(x[j]);
						}
						setFollowDone(current_nt);
					}
					else{
						if(left_cur != current_nt){
							calcFollow(getIndex(left_cur));
						}
					}
				}
				else{
					if(isNonTerminal(prod[i+1])){
						int j;
						for(j=i+1;j<prod.size();j++){
							vector<string> x = first[prod[j]];
							t_flag = false;
							for(int k=0;k<x.size();k++){
								if(x[k] == "epsilon"){
									t_flag = true;
									continue;
								}
								follow[current_nt].push_back(x[k]);
								setFollowDone(current_nt);
							}
							if(!t_flag){
								break;
							}
						}
						if(j == prod.size()){
							vector<string> x = follow[left_cur];
							for(int j=0;j<x.size();j++){
								follow[current_nt].push_back(x[j]);
							}
							setFollowDone(current_nt);
							break;
						}
					}
					else{
						follow[current_nt].push_back(string(1, prod[i+1]));
						setFollowDone(current_nt);
					}
				}
			}
		}
		it++;
	}
	//setFollowDone(current_nt);
}

void PredParser:: simplify(){
	set<string> x;
	vector<string> y;
	for(int i=0;i<nonterminals.size();i++){
		x.clear();
		y.clear();
		y = follow[nonterminals[i]];
		for(int j=0;j<y.size();j++){
			x.insert(y[j]);
		}
		follow[nonterminals[i]].clear();
		set<string>::iterator it = x.begin();
		while(it != x.end()){
			follow[nonterminals[i]].push_back(*it);
			it++;
		}
	}
}

void PredParser:: calcTable(){
	findTerminals();
	map<char, vector<string>>::iterator it = grammar.begin();
	while(it != grammar.end()){
		char left = it->first;
		vector<string> prods = it->second;
		vector<string>::iterator it1 = prods.begin();
		vector<string>::iterator fi = first[left].begin();
		for(it1 = prods.begin(); it1 != prods.end(); it1++){
			string current_prod = *it1;
			if(current_prod == "epsilon" || (isNonTerminal(current_prod[0]) && firstContainsEmpty(current_prod[0]) )){
				vector<string>::iterator it2 = follow[left].begin();
				for(it2 = follow[left].begin(); it2 != follow[left].end(); it2++){
					string pusher = string(1, left) + " -> " + current_prod;
					tuple<char, char> t = make_tuple(left, (*it2)[0]);
					table[t].push_back(pusher);
				}
			}
		}
		for(fi;fi != first[left].end();fi++){
			for(it1 = prods.begin(); it1 != prods.end(); it1++){
				if(*(it1) != "epsilon" && (!isNonTerminal((*it1)[0]) || !firstContainsEmpty((*it1)[0]))){
					string cur_prod = *it1;
					if(!isNonTerminal(cur_prod[0])){
						if((*fi)[0] == cur_prod[0]){
							string pusher = string(1, left) + " -> " + cur_prod;
							tuple<char, char> t = make_tuple(left, (*fi)[0]);
							table[t].push_back(pusher);
						}
					}
					else{
						string pusher = string(1, left) + " -> " + *it1;
						tuple<char, char> t = make_tuple(left, (*fi)[0]);
						table[t].push_back(pusher);
					}
				}
			}
		}
		it++;
	}
}

void PredParser:: printFirst(){
	cout << "NONTERMINAL\t\tFIRST\n";
	for(int i=0;i<nonterminals.size();i++){
		cout << nonterminals[i] << "\t\t";
		vector<string> first_val = first[nonterminals[i]];
		for(int i=0;i<first_val.size();i++){	
			cout << first_val[i] << " ";
		}
		cout << "\n";
	}
}

void PredParser:: printFollow(){
	cout << "NONTERMINAL\t\tFOLLOW\n";
	for(int i=0;i<nonterminals.size();i++){
		cout << nonterminals[i] << "\t\t";
		vector<string> follow_val= follow[nonterminals[i]];
		for(int i=0;i<follow_val.size();i++){	
			cout << follow_val[i] << " ";
		}
		cout << "\n";
	}
}

void PredParser:: printTable(){
	cout << "NONTERMINALS\t";
	for(int i=0;i<terminals.size();i++){
		cout << terminals[i] << "\t\t ";
	}
	cout << "\n";
	for(int i=0;i<nonterminals.size();i++){
		cout << nonterminals[i] << "\t";
		for(int j=0;j<terminals.size();j++){
			vector<string> ans = table[make_tuple(nonterminals[i], terminals[j])];
			for(int k=0;k<ans.size();k++){
				cout << ans[k] << " ";
			}
			cout << "||\t";
		}
		cout << "\n";
	}
}

void PredParser:: printParseOutput(string prod){
	for(int i=0;i<parse_stack.size();i++){
		cout << parse_stack[i];
	}
	cout << "\t\t";
	for(int i=input_ptr;i<input.size();i++){
		cout << input[i];
	}
	cout << "\t\t";
	cout << prod << "\n";
}

void PredParser:: createFollowPrereq(){
	map<char, vector<string>>::iterator it = grammar.begin();
	for(int i=0;i<nonterminals.size();i++){
		char cur = nonterminals[i];
		it = grammar.begin();
		while(it != grammar.end()){
			vector<string>::iterator it1 = it->second.begin();
			while(it1 != it->second.end()){
				string cur_string = *it1;
				for(int j=0;j<cur_string.size();j++){
					if(cur_string[j] == cur){
						pair<char, string> p;
						p.first = it->first;
						p.second = cur_string;
						follow_instances[cur].push_back(p);
					}
				}
				it1++;
			}
			it++;
		}
	}
}

bool PredParser:: parse(){
	input += "$";
	input_ptr = 0;
	parse_stack.clear();
	parse_stack.push_back('$');
	parse_stack.push_back(start);
	cout << "STACK\t\tINPUT\t\tPRODUCTION\n";
	while(parse_stack[parse_stack.size()-1] != '$'){
		char top = parse_stack[parse_stack.size()-1];
		char ip = input[input_ptr];
		if(top == ip){
			parse_stack.pop_back();
			input_ptr++;
			printParseOutput(" ");
		}
		else if(!isNonTerminal(top)){
			return false;
		}
		else if(table[make_tuple(top, ip)].size() == 0){
			return false;
		}
		else{
			string prod = table[make_tuple(top, ip)][0];
			printParseOutput(prod);
			parse_stack.pop_back();
			if(prod.substr(5,7) == "epsilon"){
				continue;
			}
			for(int i=prod.size()-1;i>-1;i--){
				if(prod[i] == ' '){
					break;
				}
				parse_stack.push_back(prod[i]);
			}
		}
	}
	if(input[input_ptr] == '$'){
		return true;
	}
	return false;
}

void PredParser:: get(){
	while(true){
		cout << "Enter the LHS of the production\n";
		cin >> left;
		if(left == '#'){
			break;
		}
		set_nonterminals.insert(left);
		cout << "Enter the RHS of the production\n";
		cin >> right;
		insert(left, right);
	}
	cout << "Enter the start symbol of the grammar\n";
	cin >> start;
	setIndex();
}

void PredParser:: startFunc(){
	cout << "Predictive Parser\n";
	cout << "Enter # while entering the LHS of the production to stop\n";
	get();
	for(int i=0;i<nonterminals.size();i++){
		first_calculation[nonterminals[i]] = false;
		follow_calculation[nonterminals[i]] = false;
	}
	for(int i=0;i<nonterminals.size();i++){
		if(!isFirstCalculated(nonterminals[i])){
			calcFirst(i);
		}
	}
	follow[start].push_back("$");
	createFollowPrereq();
	while(!isFollowDone()){
		for(int i=0;i<nonterminals.size();i++){
			if(!isFollowCalculated(nonterminals[i])){
				calcFollow(i);
			}
		}
	}
	flag = true;
	simplify();
	printFirst();
	printFollow();
	calcTable();
	printTable();
	if(flag){
		while(true){
			cout << "Enter the string to be checked for acceptance in this grammar";
			cin >> input;
			input_flag = parse();
			if(input_flag){
				cout << "The string conforms to the given context-free grammar!\n";
			}
			else{
				cout << "The string does not conform to the given context-free grammar!\n";
			}
		}
	}
	else{
		cout << "The given grammar is NOT a LL(1) grammar\n";
	}
}

int main(){
	PredParser P;
	P.startFunc();
	return 0;
}
