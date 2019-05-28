#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"hmm.h"


/*
typedef struct{
   char *model_name;
   int state_num;					//number of state
   int observ_num;					//number of observation
   double initial[MAX_STATE];			//initial prob.
   double transition[MAX_STATE][MAX_STATE];	//transition prob.
   double observation[MAX_OBSERV][MAX_STATE];	//observation prob.
} HMM;
*/

typedef struct{
	int index;
	double Pro;
}Probility;

HMM model[5];
FILE *f_model[5];
char model_name[5][50];
char name[50];
int model_num = 0;
int state_num;
int observ_length;

double my_max(double x, double y){
	if(x > y){
		return x;
	}
	return y;
}

double Viterbi(char data[], int model_index){
	double Delta[100][6];
	for(int i = 0; i < state_num; i++){
		Delta[0][i] = model[model_index].initial[i] * model[model_index].observation[data[0] - 'A'][i];
	}
	for(int i = 1; i < observ_length; i++){
		for(int j = 0; j < state_num; j++){
			Delta[i][j] = 0.0;
			for(int k = 0; k < state_num; k++){
				Delta[i][j] = my_max(Delta[i][j], Delta[i - 1][k] * model[model_index].transition[k][j] * model[model_index].observation[data[i] - 'A'][j]);
			}
		}
	}
	double Max = 0.0;
	for(int i = 0; i < state_num; i++){
		Max = my_max(Max, Delta[observ_length - 1][i]);
	}
	return Max;
}


void max_of_five(double x1, double x2, double x3, double x4, double x5, Probility *y){
	double max = 0;
	int index = 0;
	if(x1 > max){
		max = x1;
		index = 1;
	}
	if(x2 > max){
		max = x2;
		index = 2;
	}
	if(x3 > max){
		max = x3;
		index = 3;
	}
	if(x4 > max){
		max = x4;
		index = 4;
	}
	if(x5 > max){
		max = x5;
		index = 5;
	}
	y->Pro = max;
	y->index = index;
	return;
}

int main(int argc, char *argv[]){
	FILE *f_list = fopen(argv[1], "r");
	while(fscanf(f_list, "%s", name) != EOF){
		strcpy(model_name[model_num], name);
		model_num++;
	}
	for(int i = 0; i < model_num; i++){
		loadHMM(&model[i], model_name[i]);
	}
	/*
	for(int i = 0; i < model_num; i++){
		dumpHMM(stdout, &model[i]);
	}
	*/
	state_num = model[1].state_num;
	FILE *f_data = fopen(argv[2], "r");
	char data[100];

	FILE *f_result = fopen(argv[3], "w");
	while(fscanf(f_data, "%s", data) != EOF){
		observ_length = strlen(data);
		Probility y;
		max_of_five(Viterbi(data, 0), Viterbi(data, 1), Viterbi(data, 2), Viterbi(data, 3), Viterbi(data, 4), &y);
		fprintf(f_result, "model_0%d.txt %e\n", y.index, y.Pro);
	}
}
