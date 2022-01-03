#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4444

int main(){

	int sockfd, ret;
	 struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", 4444);

	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}
	
	
	
	//Generate 3 unique random numbers
	srand(time(0));
	int randNums[3];
    for (int i = 0; i < 3; i++)
    {
        bool same;
        do
        {
            same = 0;
            randNums[i] = rand() % 3 + 1;
            // Check if the newly generated number is a duplicate:
            for (int check = 0; check < i; check++)
            {
                if (randNums[i] == randNums[check])
                {
                    same = 1;
                    break;
                }
            }
        } while (same);
    }

	//**************************************//
	
	
	
	
	
	int i=0;
	int c=0;
	int socks[3];
	while(c<3)
	{
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0)
		{
			exit(1);
		}
		i=randNums[c]-1;
		socks[i]=newSocket;
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

	i++;
	c++;
	}


	

	

	
	int choice;
	i=0;
	do
	{
		printf("\n Menu (1,2 or 3, others to exit)\n");

		printf("\n 0: Exit");

		printf("\n 1: Card Reading");

		printf("\n 2: Enter Pin");

		printf("\n 3: Transaction");

		printf("\n\n Enter selection: ");

		scanf("%d", &choice);
		
		
		if(choice<4&&choice>0)
		{	i=choice-1;

			memset(buffer,0,sizeof(buffer));
			sprintf(buffer,"%d",choice);
		
			if((childpid = fork()) == 0)
			{
			  close(sockfd);



				send(socks[i], buffer, strlen(buffer), 0);
				if(strcmp(buffer, ":exit") == 0)
				{
					close(socks[i]);
					printf("[-]Disconnected from server.\n");
					exit(1);
				}
				
				if(recv(socks[i], buffer, 1024, 0) < 0)
				{
					printf("[-]Error in receiving data.\n");
				}
				else
				{
					if(strcmp(buffer,"1")==0)
						printf("Operation Successfull");
					else
						printf("Operation Failed because you have not done previous task");
				}

					
				
			
			}
		}
		else if (choice==0)
		{
			FILE *f;
			f=fopen("Account.txt","w");
			fclose(f);
			
			
			FILE *g;
			g=fopen("Status.txt","w");
			fclose(g);
			close(socks[0]);
			exit(0);
		}
	}
	while((choice > 0) && (choice< 4));

	//close(newSocket);


	return 0;
}