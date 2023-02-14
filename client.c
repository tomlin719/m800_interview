#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

double MAXSLEEP = 8.0;

int main(void){
    srand(time(NULL));
    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[2000], client_message[2000];
    int server_struct_length = sizeof(server_addr);
    double numsec = 0.5;
    char server_ip[100];
    
    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
    
    // Create socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(socket_desc < 0){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");
    
    // Set port and IP:
    printf("Enter Server IP: ");
    scanf("%s", server_ip);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    
    // Get input from the user:
    printf("Enter message: ");
    scanf("%s", client_message);
    
    for(int i=0; i<=10; i++){
    	// Send the message to server:
    	if((sendto(socket_desc, client_message, strlen(client_message), 0,
        	 (struct sockaddr*)&server_addr, server_struct_length) < 0) ||
	    (recvfrom(socket_desc, server_message, sizeof(server_message), 0,
                 (struct sockaddr*)&server_addr, &server_struct_length) < 0)){
        	printf("Unable to send or receive message\n");
    	}
        else
            break;
    	// Receive the server's response:
    	/*if(recvfrom(socket_desc, server_message, sizeof(server_message), 0,
        	 (struct sockaddr*)&server_addr, &server_struct_length) < 0){
        	printf("Error while receiving server's msg\n");
        	//return -1;
    	}*/
        if(numsec<=MAXSLEEP){
            float x = (float)rand()/(float)(RAND_MAX/numsec);
            printf("Retry after %f sec\n", x);
            printf("#########################################\n");
            sleep(x);
            printf("Retry #%d\n", i+1);
            numsec = numsec*2;
        }
        else{
            if(i==10)
                break;
            float x = (float)rand()/(float)(RAND_MAX/MAXSLEEP);
            printf("Retry after %f sec\n", x);
            printf("#########################################\n");
            sleep(x);
            printf("Retry #%d\n", i+1);
        }
        printf("Enter Server IP: ");
        scanf("%s", server_ip);
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(2000);
        server_addr.sin_addr.s_addr = inet_addr(server_ip);
    }
    
    if(strlen(server_message)==0){
	    printf("Reach max-retry and failed\n");
	    close(socket_desc);
	    return 1;
    }
    else
	    printf("Send and receive successfully and Server's response: %s\n", server_message);
    
    // Close the socket:
    close(socket_desc);
    
    return 0;
}


