#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080
#include<semaphore.h>



sem_t mutex;




void sign_in(int *new_socket){

	FILE *fptr;

	char *user_n = "User Name :";
	char *pass_n = "Password :";

	char user_name[1024]={0};
	char password[1024]={0};

	char user[1000][1000];
	char pass[1000][1000];

  	int count =-1;
	int f1 = 0,f2 = 0;
	int x,y;
	int valread;

	//user_n sent to client
	send(new_socket, user_n, strlen(user_n)+1, 0 );

	//user_name received from client
	valread = read( new_socket, user_name, 1024);
        printf("%s\n",user_name );

	//file open
	fptr = fopen("student.txt", "r");
	
	//read the file
  	while(!feof(fptr)){
  	count++;
  	fscanf(fptr, "%s %s", user[count], pass[count]);
  	}

	//check user_name
   	if(count != -1){
    		for (x=0;x<=count;x++)
    			{
    				if((strncmp(user_name, user[x], strlen(user_name))) == 0){
						f1=1;
						y=x;
				}
    			}
			
		}
		if(f1==0){
			printf("Invalid user name\n");
			sign_in(new_socket);
			return;
		}
		
  	
  	// close connection
  	fclose(fptr);


	//pass_n sent to client
	send(new_socket, pass_n, strlen(pass_n)+1, 0 );

	//password received from client
	valread = read( new_socket, password, 1024);
        printf("%s\n",password );

	
	//check password
	if((strncmp(password, pass[y], strlen(password))) == 0){
			f2=1;
			}
		if(f2==0){
			printf("Invalid password\n");
			sign_in(new_socket);
			return;
		}
	

	char *msg="sign in successfully!!!";

	//sign up confirmation massage
	send(new_socket, msg, strlen(msg)+1, 0 );
	printf("%s\n",msg);


}



void sign_up(int *new_socket){

	FILE *fptr;

	char *user_n = "User Name :";
	char *pass_n = "Password :";

	char user_name[1024]={0};
	char password[1024]={0};
	int valread;

	char user[1000][1000];
	char pass[1000][1000];
  	int count =-1;
	int x;
	
	//file open
	fptr = fopen("student.txt", "r");
	
	//read the file
  	while(!feof(fptr)){
  	count++;
  	fscanf(fptr, "%s %s", user[count], pass[count]);
  	}

	// close connection
  	fclose(fptr);

	//user_n sent to client
	send(new_socket, user_n, strlen(user_n)+1, 0 );

	//user_name received from client
	valread = read( new_socket, user_name, 1024);
        printf("%s\n",user_name );


	//check user_name
   	if(count != -1){
    		for (x=0;x<=count;x++)
    			{
    				if((strncmp(user_name, user[x], strlen(user_name))) == 0){
						printf("User name already exist\n");
						sign_up(new_socket);
						return;
						
				}
    			}
			
		}
		
  	
  	//pass_n sent to client
	send(new_socket, pass_n, strlen(pass_n)+1, 0 );

	//password received from client
	valread = read( new_socket, password, 1024);
        printf("%s\n",password );

	// open the file in append mode
  	fptr = fopen("student.txt", "a");

	// write data in file
  	fprintf(fptr, "%s %s\n", user_name, password);

  	// close connection
  	fclose(fptr);

	char *msg="signed up successfully!!!";

	//sign up confirmation massage
	send(new_socket, msg, strlen(msg)+1, 0 );
	printf("%s\n",msg);


}





void *fun(int *new_socket)
{
	sem_wait(&mutex);
	


    while(1)
    {
	
        char *hello = "Hello from server";
	char buffer[1024]={0};
	int valread;

	//value received from client into buffer
        valread = read( new_socket, buffer, 1024);
        printf("%s\n",buffer );


	if((strncmp(buffer, "Signing In", 10)) == 0){
		sign_in(new_socket);
	}
		
	else if((strncmp(buffer, "Signing Up", 10)) == 0){
		sign_up(new_socket);
	}
	else if ((strncmp(buffer, "exit", 4)) == 0) { 
	    send(new_socket, buffer, strlen(buffer)+1, 0 );
            printf("Exit server\n");
	sem_post(&mutex); 
            break; 
        }

	
	//hello msg sent to client
        send(new_socket, hello, strlen(hello)+1, 0 );
        printf("Hello message sent to client \n");
        
    }

}



int main(int argc, char const *argv[])
{
    pthread_t n0;
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
   


	sem_init(&mutex,0,10);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons( PORT );

        // Forcefully attaching socket to the port 8080
        if (bind(server_fd, (struct sockaddr *)&address,
                 sizeof(address))<0)
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        if (listen(server_fd, 3) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }


		


        while(1)
        {

	
			
            new_socket = accept(server_fd, (struct sockaddr *)&address,  (socklen_t*)&addrlen);
            
            if (new_socket<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            
              pthread_create(&n0,NULL,fun,new_socket);  
              
		
	}


	
	sem_destroy(&mutex);

       


        return 0;
    }
