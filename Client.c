#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<time.h>
#define PORT 4444



int currentbalance=0;

int main(){

	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");
	
	
	srand(time(0));
	currentbalance=rand()%50000+1;
	while(1)
	{		memset(buffer,0,sizeof(buffer));
			recv(clientSocket, buffer, 1024, 0);
			if(strcmp(buffer, ":exit") == 0)
			{
				break;
			}
			else if(strcmp(buffer, "1") == 0)
			{
				char Name[100];
				char Pin[20];
				printf("Please Set your Name:\n");
				scanf("%s",&Name[0]);
				printf("Please Set your Pin:\n");
				scanf("%s",&Pin[0]);
				
				 FILE * fp;
				 fp = fopen("Account.txt", "w");
				fputs(Pin, fp);
				fputs("\n", fp);
				fputs(Name, fp);
				memset(buffer,0,sizeof(buffer));
				sprintf(buffer,"%d",1);
				send(clientSocket, buffer, strlen(buffer), 0);
				bzero(buffer, sizeof(buffer));
				fclose(fp);
			}
			
			else if(strcmp(buffer, "2") == 0)
			{	memset(buffer,0,sizeof(buffer));
				FILE *fp;

				fp = fopen("Account.txt", "r");
				char rd[20];
				char pin[20];
				printf("Please Enter your Pin:\n");
				scanf("%s",&pin[0]);
				strcat(pin,"\n");
				fgets(rd,sizeof(rd),fp);
				strcpy(buffer,"0");


				if(strcmp(rd,pin)==0)
				{	
					strcpy(buffer,"1");
					FILE *g;
					g=fopen("Status.txt", "w");
					fputs("1",g);
					fclose(g);
				}
				fclose(fp);
				send(clientSocket, buffer, strlen(buffer), 0);
				bzero(buffer, sizeof(buffer));
			}
			
		else if(strcmp(buffer, "3") == 0)
			{	memset(buffer,0,sizeof(buffer));
					FILE *g;
					char sta[10];
					g=fopen("Status.txt", "r");
					fgets(sta,sizeof(sta),g);
					
				if(strcmp(sta,"1")==0)
				{	int tran;
					printf("Current Balance is %d:\n",currentbalance);
					printf("Please enter Amount for transaction:\n");
					scanf("%d",&tran);
					if(tran>currentbalance)
					{
						printf("transaction failed beacuse you dont have enough balance:\n");
					}
					
					else if(tran>25000)
					{
						printf("transaction failed beacuse you cant withdraw higher amount than 25000:\n");
					}
					else
					{
						currentbalance-=tran;
						printf("transaction successful\n");
						strcpy(buffer,"1");
					}
				}
				else
				{
					printf("Error.Please login first\n");
				}
				fclose(g);
				send(clientSocket, buffer, strlen(buffer), 0);
				bzero(buffer, sizeof(buffer));
			}
			
	}

	return 0;
}