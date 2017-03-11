#include<iostream>
#include<string>
#include<vector>

using namespace std;

string orig_G[8] = {
	"E->E+T", "E->E-T", "E->T", "T->T*F", "T->T/F", "T->F", "F->(E)", "F->n"};

string no_left_reg_G[10] = {
	"E->TW", "W->+TW", "W->-TW", "W->e", "T->FV", "V->*FV", "V->/FV", "V->e", "F->(E)", "F->n"};

string FIRST[5] = {
	"E:(,n", "W:+,-,e", "T:(,n", "V:*,/,e", "F:(,n"};

string FOLLOW[5] = {
	"E:$,)", "W:$,)", "T:+,-,$,)", "V:+,-,$,)", "F:*,/,+,-,$,)"};

char VN[5] = {'E', 'W', 'T', 'V', 'F'};

char VT[8] = {'+', '-', '*', '/', '(', ')', 'n', '$'};

typedef struct entry_M{
	int occupied;
	char VN;
	char VT;
	string production;
}entry_M;

entry_M table[40];

bool isVN( char ch )
{
	for ( int i = 0; i < 5; i++ ){
		if ( VN[i] == ch )  return true;
	}
	return false;
}

bool isVT( char ch )
{
	for ( int i = 0; i < 8; i++ ){
		if ( VT[i] == ch )  return true;
	}
	return false;
}

int get_VN_loc( char ch )
{
	for ( int i = 0; i < 5; i++ ){
		if ( VN[i] == ch ) return i;
	}
	return -1;
}

int get_VT_loc( char ch )
{
	for ( int i = 0; i < 8; i++ ){
		if ( VT[i] == ch ) return i;
	}
	return -1;
}

void build_table()
{
	for ( int i = 0; i < 40; i++ ){
		table[i].occupied = 0;
	}
	for ( int i = 0; i < 10; i++ ){
		if ( isVN(no_left_reg_G[i].at(3)) ){
			char VN = no_left_reg_G[i].at(3);
			int row = get_VN_loc(no_left_reg_G[i].at(0));
			for ( int j = 0; j < 5; j++ ){
				if ( FIRST[j].at(0) == VN ){
					for ( int k = 2; k < FIRST[j].length(); k++ ){
						if ( FIRST[j][k] != ',' && FIRST[j][k] != 'e' ){
							char VT = FIRST[j][k];
							int col = get_VT_loc(VT);
							table[row*8 + col].VN = no_left_reg_G[i].at(0);
							table[row*8 + col].VT = VT;
							table[row*8 + col].production = no_left_reg_G[i];
							table[row*8 + col].occupied = 1;
						}
						else if ( FIRST[j][k] == 'e' ){
							for ( int s = 0; s < 5; s++ ){
								if ( FOLLOW[s].at(0) == no_left_reg_G[i].at(0) ){
									for ( int t = 2; t < FOLLOW[s].length(); t++ ){
										if ( FOLLOW[s][t] != ',' ){
											char VT = FOLLOW[s][t];
											int col = get_VT_loc(VT);
											table[row*8 + col].VN = no_left_reg_G[i].at(0);
											table[row*8 + col].VT = VT;
											table[row*8 + col].production = no_left_reg_G[i];
											table[row*8 + col].occupied = 1;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		else if ( isVT(no_left_reg_G[i].at(3)) ){
			char VT = no_left_reg_G[i].at(3);
			int col = get_VT_loc(VT);
			int row = get_VN_loc(no_left_reg_G[i].at(0));
			table[row*8 + col].VN = no_left_reg_G[i].at(0);
			table[row*8 + col].VT = VT;
			table[row*8 + col].production = no_left_reg_G[i];
			table[row*8 + col].occupied = 1;
		}
		else if ( no_left_reg_G[i].at(3) == 'e' ){
			char VT = no_left_reg_G[i].at(3);
			int row = get_VN_loc(no_left_reg_G[i].at(0));
			for ( int s = 0; s < 5; s++ ){
				if ( FOLLOW[s].at(0) == no_left_reg_G[i].at(0) ){
					for ( int t = 2; t < FOLLOW[s].length(); t++ ){
						if ( FOLLOW[s][t] != ',' ){
							char VT = FOLLOW[s][t];
							int col = get_VT_loc(VT);
							table[row*8 + col].VN = no_left_reg_G[i].at(0);
							table[row*8 + col].VT = VT;
							table[row*8 + col].production = no_left_reg_G[i];
							table[row*8 + col].occupied = 1;
						}
					}
				}
			}
		}
	}
}

void error()
{
	cout << "Unaccepted senntence!" << endl;
}
int main()
{
	build_table();
	cout << "The analyse table is:" << endl;
	for ( int i = 0; i < 6; i++ ){
		for ( int j = 0; j < 9; j++ ){
			if ( i == 0  && j == 0) cout << '\t';
			else if ( i == 0 ){
				cout << VT[j-1] << '\t';
			}
			else if ( j == 0 ){
				cout << VN[i-1] << '\t';
			}
			else{
				cout << table[(i-1)*8+(j-1)].production << '\t';
			}
		}
		cout << endl;
		cout << "--------------------------------------------------------------------------" << endl;
	}

	string buffer;
	string buffera;
	int ip = 0;
	int top = 1;
	char X;
	char a;
	vector<char> stack;
	stack.push_back('$');
	stack.push_back('E');
	cout << "Please input the sentence:" << endl;
	cin >> buffer;
	buffera = buffer + "$";
	cout << "Stack" << '\t' << "Input" << '\t' << "Output"<< endl;
	do{
		X = stack[top];
		a = buffera.at(ip);
		if ( isVT(X) ){
			if ( X == a ){
				stack.pop_back();
				top--;
				ip++;
				for ( int i = 0; i <= top; i++ )
					cout << stack[i];
				cout << '\t';
				for ( int i = ip; i < buffera.length(); i++ )
					cout << buffera.at(i);
				cout << '\t';
			}
			else{
				error();
				break;
			}
		}
		else{
			int row = get_VN_loc(X);
			int col = get_VT_loc(a);
			if ( row < 0 || col < 0 ){
				error();
				break;
			}
			else{
				if ( table[row*8 + col].occupied != 0 ){
					stack.pop_back();
					top--;
					string production = table[row*8 + col].production;
					int upper = production.length();
					if ( production.at(upper-1) != 'e' ){
						for ( int i = upper-1; i >=3; i-- ){
							stack.push_back(production.at(i));
							top++;
						}
					}
					for ( int i = 0; i <= top; i++ )
						cout << stack[i];
					cout << '\t';
					for ( int i = ip; i < buffera.length(); i++ )
						cout << buffera.at(i);
					cout << '\t';
					cout << production;
				}
				else{
					error();
					break;
				}
			}
		}
		cout << endl;
	}while(X != '$');

	system("pause");
	return 0;
}