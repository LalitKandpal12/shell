#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include "debug.h"
#include <fcntl.h>
int childId;
int childReturnCode = 0;
void sigHandler(int signo) {
    kill(signo,childId);
    printf("\n");
}

void split(char* string, char* del, char*arr[]){
    char *token;
    token = strtok(string,del);
    int i = 0;
    while(token != NULL){
        arr[i] = token;
        token = strtok(NULL, del);
    i++;
    }
}


int main (void) {
    char hostName[10];
    gethostname(hostName,10);
    char *env = getenv("PATH");
    char *userName = getenv("USER");
    char* stringArr[15];
    split(env,":",stringArr);
    char *token ;
    int i = 0;
    int OperatorUse = 0;
    char * fileName[1];
    int newFd;
    int res, res2, res3, res4;
    int backGround = 1;

    while(1) {
        printf("%s%c%s: ",userName,'%',hostName);
        char *arguments = malloc(10*sizeof(char));
        scanf("%[^\n]%*c",arguments);
        char *argumentArr[10] ={NULL};
        split(arguments," ",argumentArr);
        int i = 0;
        while(argumentArr[i] != NULL){
            if(strcmp(argumentArr[i],">>")== 0){
                fileName[0] = argumentArr[i+1];
                argumentArr[i] = NULL;
                OperatorUse = 1;
            }
            else if(strcmp(argumentArr[i],">") == 0){
                fileName[0] = argumentArr[i+1];
                argumentArr[i] = NULL;
                OperatorUse = 2;
            }
            else if(strcmp(argumentArr[i],"&") == 0){
                argumentArr[i] = NULL;
                printf("-----: %d\n",backGround);
                backGround = 1;
                // Operator[0] = "&";
            }
            else if(strcmp(argumentArr[i],"<")==0){
                fileName[0] = argumentArr[i+1];
                argumentArr[i] = NULL;
                OperatorUse = 3;
            }
            i++;
        }

        // token = strtok(arguments," ");
        // int j = 0;
        // while(token != NULL) {
        //     if(strcmp(token, ">>")== 0){
        //         rOperator[0] = ">>";
        //         // filedes
        //         printf("break\n");
        //         token = strtok(NULL," ");
        //         fileName[1] = token;
        //         break;
        //     }
        //     else if (strcmp(token,">") == 0) {
        //         rOperator[0] = ">";
        //         printf("break\n");
        //         token = strtok(NULL," ");
        //         fileName[1] = token;
        //         break;
        //     }
        //     argumentArr[j] = token;
        //     token =  strtok(NULL," ");
        //     j++;
        // }

        // argumentArr[9] = NULL;

        int pid = fork();
        if(0 != pid){
            childId = pid;
        }
        if(signal(SIGINT,sigHandler)==SIG_ERR);
        if(0 == pid){
            if(arguments[0] == '$'&& arguments[1] == '?'){
                printf("-bash: %d: command not found\n",childReturnCode);
            }
            else if(OperatorUse != 0) {
                //if(strcmp(argumentArr[i], ">") == 0 || strcmp(argumentArr[i], ">>") == 0){
                // printf("inside loop %s this is \n",Operator[0]);
                if(OperatorUse == 1){
                    newFd = open(fileName[0], O_WRONLY| O_APPEND| O_CREAT,0644);
                    dup2(newFd,1);
                }
                else if(OperatorUse == 2){
                    newFd = open(fileName[0], O_CREAT|O_WRONLY|O_TRUNC,0644);
                    dup2(newFd,1);
                }
                else if (OperatorUse = 3){
                    newFd = open(fileName[0],O_RDONLY);
                    dup2(newFd,0);
                }
                //}
            }
            int check = 0;
            struct stat buff;
            for (int i = 0; i < 8; i++){
                char string [20];
                strcpy(string, stringArr[i]);
                strcat(string,"/");
                strcat(string, arguments);
                int ret = stat(string,&buff);
                if( ret == 0) {
                    execv(string,argumentArr);
                    check = 1;
                    break;
                }
            }
            char *args[] = {arguments,NULL};
            execv(arguments,args);
        }else if(backGround == 0) {
            int stat = 0;
            wait(&stat);
            OperatorUse = 0;
            childReturnCode = WEXITSTATUS(stat);
        }
        backGround = 0;
    }
}