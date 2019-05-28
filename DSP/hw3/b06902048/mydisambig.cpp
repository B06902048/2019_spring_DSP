#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"Ngram.h"
#include"Vocab.h"
#include"File.h"
#include"LM.h"
#include<vector>
#include<map>
#include<iostream>
#include<fstream>
#include<algorithm>

using namespace std;

int GetName(char text_name[], char map_name[], char lm_name[], int &order, int argc, char *argv[]);
void Viterbi(string line, map<string, vector<string>> Map, Ngram &lm, Vocab &v);
double W_Prob(const char w1[], const char w2[], Ngram &lm, Vocab &v);
void mapping(char map_name[], map<string, vector<string>> &Map);

int main(int argc, char *argv[]){
	//get arg
	if(argc < 9){
		printf("format error!!\n");
		exit(0);
	}
	char text_name[100], map_name[100], lm_name[100];
	int order = 0;
	if(GetName(text_name, map_name, lm_name, order, argc, argv) == 1){
		printf("format error!!\n");
		exit(0);
	}

	/*
	printf("%s\n", test_name);
	printf("%s\n", map_name);
	printf("%s\n", lm_name);
	printf("%d\n", order);
	*/

	Vocab v;
	Ngram lm(v, order);

	//mapping
	map <string, vector<string> > Map;
	mapping(map_name, Map);
	
	/*
	for(auto it : Map) {
		cout << it.first << " meowmeowmoewmoewmoewmoew ";
		for (auto x : it.second)
			cout << x << ' ';
		cout << '\n';
	}
	*/

	//read lm
	File lmFile(lm_name, "r");
	lm.read(lmFile);
	lmFile.close();
	string line;

	/*
	for(int i = Map.begin(); i != Map.end(); i++){
		printf("&s %s\n", i, Map[i]);
	}
	*/

	//read text
	ifstream f_text;
	f_text.open(text_name);
	while(getline(f_text, line)){
		line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
		Viterbi(line, Map, lm, v);
	}
	f_text.close();
	return 0;
}


double W_Prob(const char w1[], const char w2[], Ngram &lm, Vocab &v){
	VocabIndex W1 = v.getIndex(w1);
	VocabIndex W2 = v.getIndex(w2);

	//check if OOV
	if(W1 == Vocab_None){
		W1 = v.getIndex(Vocab_Unknown);
	}
	if(W2 == Vocab_None){
		W2 = v.getIndex(Vocab_Unknown);
	}

	VocabIndex C[] = { W1, Vocab_None};
	return lm.wordProb(W2, C);
}

void Viterbi(string line, map<string, vector<string>> Map, Ngram &lm, Vocab &v){
		int BT[200][2000] = {};
		double Delta[200][2000]= {{0.0}};
		string Z;
		int word_num = line.size() / 2;
		Z.assign(line.begin(), line.begin()+2);
		//printf("%s\n", Z.c_str());
		
		int N = Map[Z].size();
		//printf("%s %d\n", Z.c_str(), N);

		//initial
		for(int i = 0; i < N; i++){
			Delta[0][i] = W_Prob("<s>", Map[Z][i].c_str(), lm, v);
		}

		//iterative
		int Pre_N = N;
		string Pre_Z = Z;
		for(int i = 1; i < word_num; i++){
			Z.assign(line.begin()+2*i, line.begin()+2*i+2);
			int N = Map[Z].size();
			for(int j = 0; j < N; j++){
				double Max_Prob = -1e9;
				int Max_Index = 0;
				for(int k = 0; k < Pre_N; k++){
					double temp = Delta[i - 1][k] + W_Prob(Map[Pre_Z][k].c_str(), Map[Z][j].c_str(), lm, v); 
					if(temp > Max_Prob){
						Max_Prob = temp;
						Max_Index = k;
					}
				}
				Delta[i][j] = Max_Prob;
				BT[i][j] = Max_Index;
			}
			Pre_Z = Z;
			Pre_N = N;
		}

		//find max Prob
		double Ans_Prob = -1e9;
		int Ans_Index = 0;
		for(int i = 0; i < Pre_N; i++){
			double temp = Delta[word_num - 1][i] + W_Prob(Map[Pre_Z][i].c_str(), "</s>", lm, v); 
			if(temp > Ans_Prob){
				Ans_Prob = temp;
				Ans_Index = i;
			}
		}
		

		//backtrack
		vector<string> S;
		for(int i = word_num - 1; i >= 0; i--){
			Z.assign(line.begin()+2*i, line.begin()+2*i+2);
			S.push_back(Map[Z][Ans_Index]);
			if(i > 0){	
				Ans_Index = BT[i][Ans_Index];
			}	
		}

		//output
		reverse(S.begin(), S.end());
		printf("<s>");
		for(int i = 0; i < S.size(); i++){
			printf(" %s", S[i].c_str());
		}
		printf(" </s>\n");
		
		return;
}

int GetName(char text_name[], char map_name[], char lm_name[], int &order, int argc, char *argv[]){
	for(int i = 0; i < argc - 1; i++){
		if(strcmp(argv[i], "-text") == 0){
			if(strlen(argv[i+1]) > 90){
				printf("test name is too long\n");
				return 1;
			}
			strcpy(text_name, argv[i+1]);
		}
		else if(strcmp(argv[i], "-map") == 0){
			if(strlen(argv[i+1]) > 90){
				printf("map name is too long\n");
				return 1;
			}
			strcpy(map_name, argv[i+1]);
		}
		else if(strcmp(argv[i], "-lm") == 0){
			if(strlen(argv[i+1]) > 90){
				printf("lm name is too long\n");
				return 1;
			}
			strcpy(lm_name, argv[i+1]);
		}
		else if(strcmp(argv[i], "-order") == 0){
			if(atoi(argv[i+1]) > 3){
				printf("order is too big\n");
				return 1;
			}
			order = atoi(argv[i+1]);
		}
	}
	return 0;
}

void mapping(char map_name[], map <string, vector<string>> &Map){
	ifstream f_map;
	f_map.open(map_name);
	string line;
	while(getline(f_map, line)){
		int l = strlen(line.c_str());
		string Z;
		string W;	
		Z.assign(line.begin(), line.begin()+2);
		//printf("%s\n", line.c_str());
		for(int i = 3; i < l; i+=3){
			string w;
			getline(f_map, w);
			W.assign(line.begin()+i, line.begin()+i+2);
			Map[Z].push_back(W);
			if(Map.count(W) == 0){
				Map[W].push_back(W);
			}
			//printf("%s\t%s\n", W.c_str(), W.c_str());
		}
	}
}
