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
		bool input_flag;
		bool ep_flag;
		set<char> set_nonterminals;
		set<char> set_terminals;
		vector<char> nonterminals;
		vector<char> terminals;
		map<char, bool> first_calculation;
		map<char, bool> follow_calculation;
		map<char, vector<string>> grammar;
		map<char, vector<pair<char,string>>> follow_instances;
		map<char, vector<string>> first;
		map<char, vector<string>> follow;
		map<char, map<string, vector<string>>> table;
		bool isNonTerminal(char x);
		int getIndex(char x);
		void setIndex();
		void calcFirst();
		void calcFollow();
		void calcTable();
		void printFirst();
		void printFollow();
		void printTable();
		void printParserOutput();
		void setFirstDone(char x);
		bool isFirstDone();
		bool isFirstCalculated(char x);
		bool isFollowCalculated(char x);
		void insert(char left, string right);
		void findTerminals();
		void createFollowPrereq();
		void parse(string x);
		void get();
	public:
		void start();
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
					set_terminals.push_back(cur[j]);
				}
			}
		}
		it++;
	}
	for(auto x:set_terminals){
		terminals.push_back(x);
	}
}

void PredParser:: setFirstDone(char x){
	first_calculation[x] = true;
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

bool PredParser:: isFirstCalculated(char x){
	bool ans = first_calculation[x];
	return ans;	
}

bool PredParser:: isFollowCacluated(char x){
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
	set<char>::iterator it = unique_nonterminals.begin();
	while(it!=unique_nonterminals.end()){
		if(*it != start){
			nonterminals.push_back(*it);
		}
		it++;
	}
}

bool PredParser:: isNonTerminal(char x){
	if(unique_nonterminals.find(x) == unique_nonterminals.end()){
		return false;
	}
	return true;
}

void PredParser:: insert(char left, string right){
	grammar[left].push_back(right);
}

void PredParser:: calcFirst(int index){
	char current_nt = nonterminals[index];// sort out the logic here!
	vector<string> current_char_prods = grammar[current_nt];
	string current_prod;
	char first_char;
	for(int i=0; i<current_char_prods.size();i++){
		current_prod = current_char_prods[i];
		if(current_prod == "epsilon"){
			continue;
		}
		for(int j=0;j<current_prod.size();j++){
			ep_flag = false;
			current_char = current_prod[j];
			if(isNonTerminal(current_char)){
				if(first_caculation[current_char]){
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
				first[current_nt].push_back(first_char);
			}
		}
	}
	setFirstDone(current_nt);
	calcFirst((getIndex(current_nt) + 1);
}

void PredParser:: calcFollow(int index){
	char current_nt = nonterminals[index];
	vector<pair<char,string>> = follow_instances[current_nt];
	vector<pair<char,string>>::iterator it = follow_instances.begin();
	while(it != follow_instances.end()){
		int i = 0;
		string left_cur = (*it)->first;
		string prod = (*it)->second;
		for(int i=0;i<prod.size();i++){
			if(prod[i] == current_nt){
				if(i == (prod.size()-1)){
					if(isFollowCalculated(left_cur)){
						vector<string> x = follow[left_cur];
						for(int i=0;i<x.size();i++){
							follow[current_nt].push_back(x[i]);
						}
					}
					else{
						calcFollow(getIndex(left_cur));
					}
				}
				else{
					if(isNonTerminal(prod[i+1])){
						vector<string> x = first[prod[i+1]];
						for(int i=0;i<x.size();i++){
							follow[current_nt].push_back(x[i]);
						}
					}
					else{
						follow[current_nt].push_back(prod[i+1]);
					}
				}
			}
		}
		it++;
	}
}

void PredParse:: calcTable(){
	map<char, vector<string>>::iterator it = grammar.begin();
	bool flag = false;
	while(it != grammar.end()){
		char left = it->first;
		flag = false;
		vector<string>::iterator fi = first[left].begin();
		for(fi;fi != first[left].end();fi++){
			if(*fi == "epsilon"){
				flag = true; 
			}
			string pusher = string
			table[left][*fi].push_back();
		}
		it++;
	}
}

void printFirst(){

}

void printFollow(){

}

void printTable(){

}

void PredParser:: createFollowPrereq(){
	map<char, vector<string>>::iterator it = grammar.begin();
	for(int i=0;i<nonterminals.size();i++){
		char cur = nonterminals[i];
		it = grammar.begin();
		while(it != grammar.end()){
			vector<string>::iterator it1 = it->second.begin();
			while(it1 != it->second.begin()){
				string cur_string = *it1;
				for(int j=0;j<cur_string.size();j++){
					if(cur_string[j] == 'cur'){
						pair<char, string> p;
						p.first = it->first;
						p.second = cur_string;
						follow_instances[cur].push_back(p);
						continue;
					}
				}
				it1++;
			}
			it++;
		}
	}
}

void PredParser:: get(){
	while(true){
		cout << "Enter the LHS of the production\n";
		cin >> left;
		if(left == '#'){
			break;
		}
		cout << "Enter the RHS of the production\n";
		cin >> right;
		insert(left, right);
	}
	cout << "Enter the start symbol of the grammar\n";
	cin >> start;
	setIndex();
}

void PredParser:: start(){
	cout << "Predictive Parser\n";
	cout << "Enter # while entering the LHS of the production to stop\n";
	get();
	for(int i=0;i<nonterminals.size();i++){
		first_calculation[nonterminals[i]] = false;
		follow_calculation[nonterminals[i]] = false;
	}
	//probably get this into a method
	for(int i=0;i<nonterminals.size();i++){
		if(!isFirstCalculated(nonterminals[i])){
			calcFirst(i);
		}
	}
	follow[start].push_back('$');
	for(int i=0<nonterminals.size();i++){
		if(!isFollowCalculated(nonterminals[i])){
			calcFollow(i);
		}
	}
	printFirst();
	printFollow();
	calcTable();
	printTable();
	if(flag){
		while(true){
			cout << "Enter the string to be checked for acceptance in this grammar";
			cin >> input;
			input_flag = parse(input);
			if(input_flag){
				printParseOutput();
				cout << "The string conforms to the given context-free grammar!\n";
			}
			else{
				cout << "The string does not conform to the given context-free grammar!\n";
			}
		}
	}
	else{
		cout << "The given grammar is NOT a LL(1) grammar\n":
	}
}

int main(){
	PredParser P;
	P.start();
	return 0;
}
