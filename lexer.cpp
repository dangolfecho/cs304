// conditional operator
#include<bits/stdc++.h>
using namespace std;

class Lexer{
	private:
		vector<pair<string,string>> result;
		unordered_set<string> keywords;
		unordered_set<string> operators;
		string filename;
		string store;
		int update_val;
		vector<string> lines;
		vector<string> lines_wospace;
		vector<string> tokenized_lines;
		vector<string>:: iterator it;
		bool isAlphaNum(char c);
		void initKeywords();
		void initOperators();
		void insertKeyword(string text);
		void insertOperator(string text);
		bool isKeyword(string given, int index);
		string getKeyword(string given, int index);
		bool isOperator(string given, int index);
		string getOperator(string given, int index);
		bool isPunctuator(string given, int index);
		string getPunctuator(string given, int index);
		bool isDelimiter(string given, int index);
		string getDelimiter(string given, int index);
		bool isIdentifier(string given, int index);
		string getIdentifier(string given, int index);
		bool isConstant(string given, int index);
		string getConstant(string given, int index);
		void get();
		void tokenize();
	public:
		void menu();
		void run();
		void printDetails();
};

bool Lexer:: isAlphaNum(char c){
	if(c >= '0' && c <='9'){
		return true;
	}
	else if(c >= 'a' && c <= 'z'){
		return true;
	}
	else if(c >= 'A' && c <= 'Z'){
		return true;
	}
	return false;
}

void Lexer:: insertKeyword(string text){
	string insertee = text.substr(0, text.size());
	keywords.insert(insertee);
}

void Lexer:: insertOperator(string text){
	string insertee = text.substr(0, text.size());
	operators.insert(insertee);
}

void Lexer:: initOperators(){
	string line;
	ifstream f("operators.txt");
	if(f.is_open()){
		while(getline(f, line)){
			insertOperator(line);
		}
		f.close();
	}
}

void Lexer:: initKeywords(){
	string line;
	ifstream f("keywords.txt");
	if(f.is_open()){
		while(getline(f, line)){
			insertKeyword(line);
		}
		f.close();
	}
}

bool Lexer:: isKeyword(string given, int index){
	int i;
	for(i = index;i<given.size();i++){
		if(given[i] == ' '){
			break;
		}
	}
	if(keywords.find(given.substr(index, i-index)) != keywords.end()){
		store = given.substr(index, i-index);
		update_val = i-index-1;
		return true;
	}
	return false;
}

string Lexer:: getKeyword(string given, int index){
	return store;
}

bool Lexer:: isOperator(string given, int index){
	int i;
	for(i = index;i<given.size();i++){
		if(given[i] == ' ' || isAlphaNum(given[i])){
			break;
		}
	}
	if(operators.find(given.substr(index, i-index)) != operators.end()){
		store = given.substr(index, i-index);
		update_val = i-index-1;
		return true;
	}
	return false;
}

string Lexer:: getOperator(string given, int index){
	return store;
}

bool Lexer:: isPunctuator(string given, int index){
	if(given[index] == '{' || given[index] == '}' || given[index] == '(' || given[index] == ')' || given[index] == '[' || given[index] == ']' || given[index] == ','){
		store = given[index];
		update_val = 0;
		return true;
	}
	return false;
}

string Lexer:: getPunctuator(string given, int index){
	return store;
}

bool Lexer:: isIdentifier(string given, int index){
	if(given[index] >= '0' && given[index] <= '9'){
		return false;
	}
	int i;
	for(i = index;i<given.size();i++){
		if(isAlphaNum(given[i]) || given[i] == '_'){
			continue;
		}
		else{
			break;
		}

	}
	if(i != index){
		store = given.substr(index, i-index);
		update_val = i-index-1;
		return true;
	}
	return false;
}

string Lexer:: getIdentifier(string given, int index){
	return store;
}

bool Lexer:: isConstant(string given, int index){
	int i;
	for(i = index;i<given.size();i++){
		if((given[i] >= 'a' && given[i] <= 'z') || (given[i] >= 'A' && given[i] <= 'Z')){
			return false;
		}
		else if(given[i] == ' ' || given[i] == ',' || given[i] == ';'){
			break;
		}
	}
	store = given.substr(index, i-index);
	update_val = i-index-1;
	return true;
}

string Lexer:: getConstant(string given, int index){
	return store;
}

void Lexer:: get(){
	string line;
	ifstream f(filename);
	if(f.is_open()){
		while(getline(f, line)){
			lines.push_back(line);
		}
		f.close();
	}
}

void Lexer:: tokenize(){
	initKeywords();
	initOperators();
	string temp;
	bool multi = false;
	for(it = lines.begin(); it != lines.end(); it++){
		temp = *it;
		for(int i = 0; i < temp.size(); i++){
			if(temp[i] == ' ' || temp[i] == '\t'){
				continue;
			}
			else if(temp[i] == '/' && (i+1) < temp.size() && temp[i+1] == '/'){
				break;
			}
			else if(temp[i] == '/' && (i+1) < temp.size() && temp[i+1] == '*'){
				multi = true;
			}
			else if(temp[i] == '*' && (i+1) < temp.size() && temp[i+1] == '/'){
				multi = false;
				i += 1;
				continue;
			}
			if(multi){
				continue;
			}
			else if(i == 0 && temp[i] == '#'){
				if(temp.substr(i, 8) == "#include"){
					result.push_back({"header file", temp});
					break;
				}
				else if(temp.substr(i, 7) == "#define"){
					result.push_back({"macro", temp});
					break;
				}
			}
			else if(temp[i] == '"'){
				int j;
				for(j = i+1;j<temp.size(); j++){
					if(temp[j] == '"'){
						break;
					}
				}
				j++;
				string newone  = temp.substr(i, j-i);
				result.push_back({"literal", newone});
				i += j-i-1;
			}
			else if(isKeyword(temp, i)){
				string x = getKeyword(temp, i);
				result.push_back({"keyword", x});
				i += update_val;
			}
			else if(isOperator(temp, i)){
				string x = getOperator(temp, i);
				result.push_back({"operator", x});
				i += update_val;
			}
			else if(isPunctuator(temp, i)){
				string x = getPunctuator(temp, i);
				result.push_back({"punctuator", x});
			}
			else if(temp[i] == ';'){
				result.push_back({"delimiter", ";"});
			}
			else if(isIdentifier(temp, i)){
				string x = getIdentifier(temp, i);
				result.push_back({"identifier", x});
				i += update_val;
			}
			else if(isConstant(temp, i)){
				string x = getConstant(temp, i);
				result.push_back({"constant", x});
				i += update_val;
			}
		}
	}
}

void Lexer:: menu(){
	cout << "Enter the filename: ";
	cin >> filename;
}

void Lexer:: run(){
	get();
	tokenize();
}

void Lexer:: printDetails(){
	int count = result.size();
	cout << "The total number of tokens are: " << count << ".\n";
	cout << "The tokens are: \n";
	vector<pair<string, string>>:: iterator it;
	for(it = result.begin(); it != result.end(); it++){
		cout << it->first << ": " << it->second << "\n";
	}
}

int main(){
	Lexer l;
	l.menu();
	l.run();
	l.printDetails();
	return 0;
}
