#include<bits/stdc++.h>
using namespace std;

int calc_worth(set<char> nt, vector<string> v){
	int val = 0;
	for(auto x:v){
		if(x == "epsilon"){
			continue;
		}
		if(nt.find(x[0]) == nt.end()){
			val++;
		}
	}
	return val;
}

char getBest(set<char> done, map<char, int> m){
	int min = 1000;
	char ans;
	for(auto x:m){
		if(done.find(x.first) == done.end()){
			if(x.second < min){
				min = x.second;
				ans = x.first;
			}
		}
	}
	return ans;
}

int main(){
	cout << "Enter the grammar\n";
	cout << "Enter # to stop\n";
	cout << "Enter epsilon to signify empty string\n";
	map<char, vector<string>> m;
	set<char> nt;
	map<char,int> nt_worth;
	char left;
	char start;
	string right;
	while(true){
		cout << "Enter the LHS of the production\n";
		cin >> left;
		if(left == '#'){
			break;
		}
		cout << "Enter the RHS of the production\n";
		cin >> right;
		m[left].push_back(right);
		nt.insert(left);
	}
	cout << "Enter which NT is the start symbol\n";
	cin >> start;
	for(auto x:nt){
		nt_worth[x] = calc_worth(nt, m[x]);
	}
	map<char, vector<pair<string, char>>> reverse;
	for(auto x:m){
		for(auto y:x.second){
			for(int i=0;i<y.size();i++){
				if(nt.find(y[i]) != nt.end()){
					pair<string, char> p;
					p.first = y;
					p.second=x.first;
					reverse[y[i]].push_back(p);
				}
			}
		}
	}
	map<char, vector<string>> first;
	map<char, vector<string>> follow;
	follow[start].push_back("$");
	set<char> done;
	for(int i=0;i<nt.size();i++){
		left = getBest(done, nt_worth);
		done.insert(left);
		vector<string> prod = m[left];
		string current_prod;
		for(int i=0;i<prod.size();i++){
			current_prod = prod[i];
			if(current_prod == "epsilon"){
				first[left].push_back("epsilon");
				continue;
			}
			if(nt.find(current_prod[0]) == nt.end()){
				string ans = string(1,current_prod[0]);
				first[left].push_back(ans);
			}
			else{
				bool f = false;
				for(int i=0;i<current_prod.size();i++){
					for(auto x:first[current_prod[i]]){
						if(x == "epsilon"){
							f = true;
						}
						first[left].push_back(x);
					}
					if(!f){
						break;
					}
				}
			}
		}
	}
	for(auto x:nt){
		vector<pair<string, char>> v = reverse[x];
		string prod;
		char left;
		for(int i=0;i<v.size();i++){
			prod = v[i].first;
			left = v[i].second;
			if(prod == "epsilon"){
				follow[x].push_back("epsilon");
				continue;
			}
			bool f = false;
			for(int i=0;i<prod.size();i++){
				f = false;
				if(prod[i] == x){
					if(i == prod.size()-1){
						for(auto y:follow[left]){
							follow[x].push_back(y);
						}
					}
					else{
						if(nt.find(prod[i]) == nt.end()){
							follow[x].push_back(string(1,prod[i]));
							break;
						}
						else{
							for(auto y:first[prod[i]]){
								if(y == "epsilon"){
									f = true;
								}
								follow[x].push_back(y);
							}
						}
					}
				}
				if(!f){
					break;
				}
			}
		}
	}
	cout << "NONTERMINAL\t\tFIRST()\t\tFOLLOW()\n";
	for(auto x:nt){
		cout << x << " ";
		vector<string> fi = first[x];
		vector<string> fo = follow[x];
		for(auto x:fi){
			cout << x << " ";
		}
		cout << "\t\t";
		for(auto x:fo){
			cout << x << " ";
		}
		cout << "\n";
	}
	return 0;
}
