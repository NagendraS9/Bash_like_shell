#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64
#define MAX_FOREGROUND_PROCESS 64
// TODO: YOUR CODE HERE
// INITIALIZE DATA STRUCTURE TO STORE PIDS OF PROCESSES TO KILL LATER

/* Splits the string by space and returns the array of tokens
*
*/
int pids[MAX_FOREGROUND_PROCESS]={0};
int curId=0;
char **tokenize(char *line) {

    // tokenizer variables
	char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
	char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
	int i, tokenIndex = 0, tokenNo = 0;

    // loop on length of line
	for(i=0; i < strlen(line); i++){

		char readChar = line[i];

        // tokenize on any kind of space
		if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
			token[tokenIndex] = '\0';
			if (tokenIndex != 0) {
				tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
				strcpy(tokens[tokenNo++], token);
				tokenIndex = 0; 
			}
		}
		else {
			token[tokenIndex++] = readChar;
		}
	}
	
	free(token);
	tokens[tokenNo] = NULL ;
	return tokens;
}

// TODO
// MAKE FUNCTIONS AS REQUIRED TO AVOID REPETITIVE WORK
void ExecuteSingleCommand(char** tokens){
	char path[128];
	const char* x="/bin/";
	strcpy(path,x);
	strcat(path,tokens[0]);
	execv(path,tokens);
}
int main(int argc, char* argv[]) {

	char  line[MAX_INPUT_SIZE];            
	char  **tokens;           

	// TODO: YOUR CODE HERE
	// INITIALIZE GLOBAL DATA STRUCTURE OF PIDS TO SOME DEFAULT VALUE  
	// INITIALIZE OTHER VARIABLES AS NEEDED
	for(int i=0;i<MAX_FOREGROUND_PROCESS;i++){
		pids[i]=-1;
	}

	while(1) {	

		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		printf("$ ");
		scanf("%[^\n]", line);
		getchar();
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);

		// You can maintain boolean variables to check which type of command is given by user and then
        // conditionally execute depending on them
		if(strcmp(tokens[0],"exit")==0){
			for(int i=0;i<MAX_FOREGROUND_PROCESS;i++){
				if(pids[i]!=-1){
					kill(pids[i], SIGTERM);
				}
			}
			exit(0);
		}
		bool hasDoubleAmp=false;
		bool hasTripleAmp=false;
		int i=0;
		while(tokens[i]!=NULL){
			if(strcmp(tokens[i],"&&")==0){
				hasDoubleAmp=true;break;
			}
			if(strcmp(tokens[i],"&&&")==0){
				hasTripleAmp=true;break;
			}
			i++;
		}

		if(hasDoubleAmp){
			int noOfDoubleAmp=0;
			char ***separatedTokens=(char ***)malloc(64 * sizeof(char **)); //max 64 running

			int end=0;
			int start=0;
			while(tokens[end]!=NULL){
				if(strcmp(tokens[end],"&&")==0){
					separatedTokens[noOfDoubleAmp]=(char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
					memcpy(separatedTokens[noOfDoubleAmp], &tokens[start], sizeof(char*) * (end-start));
					separatedTokens[noOfDoubleAmp][end-start]=NULL;

					noOfDoubleAmp++;
					start=end+1;
				}
				end++;
			}

			separatedTokens[noOfDoubleAmp]=(char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
			memcpy(separatedTokens[noOfDoubleAmp], &tokens[start], sizeof(char*) * (end-start));
			separatedTokens[noOfDoubleAmp][end-start]=NULL;

			// printf("%s\n",separatedTokens[1][0]);
			for(int i=0;i<=noOfDoubleAmp;i++){
				if(strcmp(separatedTokens[i][0],"cd")==0){
					if(separatedTokens[i][1]==NULL){
						printf("Please Mention path\n");
					}
					else if(separatedTokens[i][2]!=NULL){
						printf("Correct usage: \"cd dir\"\n");
					}
					else{
						if (chdir(separatedTokens[i][1]) != 0) 
							perror("error");
					}
				}
				else{
					int pid=fork();
					if(pid==0){
						ExecuteSingleCommand(separatedTokens[i]);
						exit(0);			
					}
					else{
						pids[curId]=pid;
						curId++;
						curId%=MAX_FOREGROUND_PROCESS;
						wait(NULL);
					}
				}
			}
		}

		else if(hasTripleAmp){
			int noOfTripleAmp=0;
			char ***separatedTokens=(char ***)malloc(64 * sizeof(char **)); //max 64 running

			int end=0;
			int start=0;
			while(tokens[end]!=NULL){
				if(strcmp(tokens[end],"&&&")==0){
					separatedTokens[noOfTripleAmp]=(char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
					memcpy(separatedTokens[noOfTripleAmp], &tokens[start], sizeof(char*) * (end-start));
					separatedTokens[noOfTripleAmp][end-start]=NULL;

					noOfTripleAmp++;
					start=end+1;
				}
				end++;
			}
			separatedTokens[noOfTripleAmp]=(char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
			memcpy(separatedTokens[noOfTripleAmp], &tokens[start], sizeof(char*) * (end-start));
			separatedTokens[noOfTripleAmp][end-start]=NULL;

			// printf("%s\n",separatedTokens[1][0]);
			for(int i=0;i<=noOfTripleAmp;i++){
				if(strcmp(separatedTokens[i][0],"cd")==0){
					printf("Can't change directory in parallel execution");
				}
				else{
					int pid=fork();
					if(pid==0){
						ExecuteSingleCommand(separatedTokens[i]);
						exit(0);	
					}
					else{
						pids[curId]=pid;
						curId++;
						curId%=MAX_FOREGROUND_PROCESS;
					}
				}
			}
			while(wait(NULL)>0){}
		}
		else{
			if(strcmp(tokens[0],"cd")==0){
				if(tokens[1]==NULL){
					printf("Please Mention path\n");
				}
				else if(tokens[2]!=NULL){
					printf("Correct usage: \"cd dir\"\n");
				}
				else{
					if (chdir(tokens[1]) != 0) 
						perror("error");
				}
			}
			else{
				int pid=fork();
				if(pid==0){
					ExecuteSingleCommand(tokens);
				}
				else{
					pids[curId]=pid;
					curId++;
					curId%=MAX_FOREGROUND_PROCESS;
					wait(NULL);
				}
			}
		}


        // TODO: YOUR CODE HERE
        // TODO: YOUR CODE HERE
    
        // freeing the memory
		for(int i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}

		free(tokens);

	}
	return 0;
}


