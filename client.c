// Client side C/C++ program to demonstrate Socket programming .
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#define PORT 8080 

int main(int argc, char const *argv[]) 
{ 
	int sock = 0, valread; 
	struct sockaddr_in serv_addr; 
	 
	char buffer[1024] = {0}; 
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	}


	int o=-1;
	int n;
	printf("Press 1: Sign In\nPress 2: Sign Up\n");
	while(1){
		scanf("%d",&n);
		switch(n){
			case 1: printf("Signing In\n");
				char *signin="Signing In";
				send(sock , signin , strlen(signin) , 0 );
				o=1;
				break;
			case 2: printf("signing Up\n");
				char *signup="Signing Up";
				send(sock , signup , strlen(signup) , 0 );
				o=1;
				break;
			default: printf("wrong input\n");
				
}
		if(o==1)break;
}

	
	
	while(1){

	//value received from server
	valread = read( sock , buffer, 1024); 

	if ((strncmp(buffer, "exit", 4)) == 0) { 
            printf("Exit client\n"); 
            break; 
        } 

	//print received value 
	printf("%s\n",buffer ); 

	 		
	char *hello;
	scanf("%s",hello); 

	//value sent to server
	send(sock , hello , strlen(hello) , 0 ); 
	//printf("%s message sent to server\n",hello); 
		
      	
	}

	return 0; 
} 


