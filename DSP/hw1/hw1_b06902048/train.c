#include"hmm.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

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


char test_data[10000 + 7][100];
HMM init_model;
double Alpha[10000 + 7][50 + 7][6] = {};
double Beta[10000 + 7][50 + 7][6] = {};
double Gamma[10000 + 7][50 + 7][6] = {};
double Epsilon[10000 + 7][50 + 7][6][6] = {};
double accu_Gamma_for_Pi[6] = {};
double accu_Epsilon_for_a[6][6] = {};
double accu_Gamma_for_a[6] = {};
double accu_Gamma_for_b[6][6] = {};
int observ_length = 0;
int state_num = 6;
int n = 0;
int O_num = 6;

int O_trans(int i, int j){
	return test_data[i][j] - 'A';
}

void count_Alpha(int n){
	for(int i = 0; i < state_num; i++){
		Alpha[n][0][i] = init_model.initial[i] * init_model.observation[O_trans(n, 0)][i];
	}
	for(int i = 1; i < observ_length; i++){
		for(int j = 0; j < state_num; j++){
			Alpha[n][i][j] = 0.0;
			for(int k = 0; k < state_num; k++){
				Alpha[n][i][j] += Alpha[n][i-1][k] * init_model.transition[k][j] * init_model.observation[O_trans(n, i)][j];
			}
		}
	}
}

void count_Beta(int n){
	for(int i = 0; i < state_num; i++){
		Beta[n][observ_length - 1][i] = 1.0;
	}
	for(int i = observ_length - 2; i >= 0; i--){
		for(int j = 0; j < state_num; j++){
			Beta[n][i][j] = 0.0;
			for(int k = 0; k < state_num; k++){
				Beta[n][i][j] += Beta[n][i + 1][k] * init_model.transition[j][k] * init_model.observation[O_trans(n, i + 1)][k];
			}
		}
	}
}

void count_Gamma(int n){
	for(int i = 0; i < observ_length; i++){
		double sum = 0.0;
		for(int j = 0; j < state_num; j++){
			sum += Alpha[n][i][j] * Beta[n][i][j];
		}
		for(int j = 0; j < state_num; j++){
			Gamma[n][i][j] = Alpha[n][i][j] * Beta[n][i][j] / sum;
			if(i == 0){
				accu_Gamma_for_Pi[j] += Gamma[n][0][j];
			}
			if(i != observ_length - 1){
				accu_Gamma_for_a[j] += Gamma[n][i][j];
				accu_Gamma_for_b[O_trans(n, i)][j] +=  Gamma[n][i][j];
			}
		}
	}

}

void count_Epsilon(int n){
	for(int i = 0; i < observ_length - 1; i++){
		double sum = 0.0;
		for(int j = 0; j < state_num; j++){
			for(int k = 0; k < state_num; k++){
				sum += Alpha[n][i][j] * init_model.transition[j][k] * init_model.observation[O_trans(n, i + 1)][k] * Beta[n][i + 1][k];
			}
		}
		for(int j = 0; j < state_num; j++){
			for(int k = 0; k < state_num; k++){
				Epsilon[n][i][j][k] = Alpha[n][i][j] * init_model.transition[j][k] * init_model.observation[O_trans(n, i + 1)][k] * Beta[n][i + 1][k] / sum;
				accu_Epsilon_for_a[j][k] += Epsilon[n][i][j][k];
			}
		}
	}
}

void count_Pi(){
	for(int i = 0; i < state_num; i++){
		init_model.initial[i] = accu_Gamma_for_Pi[i] / n;
	}
}

void count_a(){
	for(int i = 0; i < state_num; i++){
		for(int j = 0; j < state_num; j++){
			init_model.transition[i][j] = accu_Epsilon_for_a[i][j] / accu_Gamma_for_a[i];
		}
	}
}

void count_b(){
	for(int i = 0; i < O_num; i++){
		for(int j = 0; j < state_num; j++){
			init_model.observation[i][j] = accu_Gamma_for_b[i][j] / accu_Gamma_for_a[j];
		}
	}
}

int main(int argc, char *argv[]){
	loadHMM(&init_model, argv[2]);
	//dumpHMM(stdout, &init_model);
	//printf("init_model->state_num = %d\n", init_model.state_num);
	//printf("observ_num = %d\n", init_model.observ_num);
	FILE *f = fopen(argv[3], "r");
	if(f == NULL){
		printf("open error\n");
		exit(0);
	}

	while(fscanf(f, "%s", test_data[n]) != EOF){
		n++;
	}

	/*
	for(int i = 0; i < n; i++){
		printf("%s\n", test_data[i]);
	}
	printf("%d\n", n);
	*/

	int iteration = atoi(argv[1]);
	observ_length = strlen(test_data[0]);
	for(int i = 0; i < iteration; i++){
		for(int j = 0; j < state_num; j++){
			for(int k = 0; k < state_num; k++){
				accu_Epsilon_for_a[j][k] = 0.0;
				accu_Gamma_for_b[j][k] = 0.0;
			}
			accu_Gamma_for_Pi[j] = 0.0;
			accu_Gamma_for_a[j] = 0.0;
		}
		for(int j = 0; j < n; j++){
			count_Alpha(j);	
			count_Beta(j);
			count_Gamma(j);
			count_Epsilon(j);
		}
		count_Pi();
		count_a();
		count_b();
	}
	FILE *f_new = fopen(argv[4], "w");
	dumpHMM(f_new, &init_model);
}
